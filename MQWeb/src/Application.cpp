/*
* Copyright 2010 MQWeb - Franky Braem
*
* Licensed under the EUPL, Version 1.1 or – as soon they
* will be approved by the European Commission - subsequent
* versions of the EUPL (the "Licence");
* You may not use this work except in compliance with the
* Licence.
* You may obtain a copy of the Licence at:
*
* http://joinup.ec.europa.eu/software/page/eupl
*
* Unless required by applicable law or agreed to in
* writing, software distributed under the Licence is
* distributed on an "AS IS" basis,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied.
* See the Licence for the specific language governing
* permissions and limitations under the Licence.
*/
#include "MQ/MQSubsystem.h"
#include "MQ/Web/Version.h"
#include "MQ/Web/Application.h"
#include "MQ/Web/RequestHandlerFactory.h"
#include "MQ/Web/WebSocketRequestHandler.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"

#include "Poco/Data/SQLite/Connector.h"

#include "Poco/Logger.h"
#include "Poco/File.h"

#include <iostream>

using namespace Poco::Net;
using namespace Poco::Util;


MQWebApplication::MQWebApplication()
: _helpRequested(false)
, _versionRequested(false)
{
	addSubsystem(new MQ::MQSubsystem());
}

MQWebApplication::~MQWebApplication()
{
}

void MQWebApplication::initialize(Application& self)
{
	try
	{
		if (config().has("mq.web.cfg"))
		{
				loadConfiguration(config().getString("mq.web.cfg"));
		}
		else
		{
				loadConfiguration(); // load default configuration files, if present
		}
	}
	catch(Poco::FileException& fe)
	{
		std::cout << "Caught a file exception when loading configuration file: " << fe.message() << std::endl;
	}

	Poco::Data::SQLite::Connector::registerConnector();

	try
	{
		ServerApplication::initialize(self);
	}
	catch(Poco::NotFoundException& nfe)
	{
		// We can get here when some log configuration is not available
		std::cout << "Please check your logger configuration because we got a NotFoundException for " << nfe.message() << std::endl;
	}
}

void MQWebApplication::uninitialize()
{
	Poco::Logger::get("mq.web").information("MQWeb process stopped!");

	Poco::Data::SQLite::Connector::unregisterConnector();

	ServerApplication::uninitialize();
}

void MQWebApplication::defineOptions(Poco::Util::OptionSet& options)
{
	ServerApplication::defineOptions(options);

	options.addOption(
	Option("help", "h", "display help information on command line arguments")
		.required(false)
		.repeatable(false));

	options.addOption(
	Option("version", "v", "display version")
		.required(false)
		.repeatable(false));

	options.addOption(
	Option("qmgr", "m", "connect only to this queuemanager")
		.required(false)
		.repeatable(false)
		.binding("mq.web.qmgr")
		.argument("<name>"));

	options.addOption(
	Option("config", "c", "Configuration file to load")
		.required(false)
		.repeatable(false)
		.binding("mq.web.cfg")
		.argument("<propertyfile>"));

	options.addOption(
	Option("port", "p", "Port for HTTP listener")
		.required(false)
		.repeatable(false)
		.binding("mq.web.port")
		.argument("<port>"));
}

void MQWebApplication::handleOption(const std::string& name, const std::string& value)
{
	ServerApplication::handleOption(name, value);

	if (name == "help")
	{
		_helpRequested = true;
	}

	if ( name == "version" )
	{
		_versionRequested = true;
	}
}

void MQWebApplication::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("A web server for managing Websphere MQ.");
	helpFormatter.format(std::cout);
	displayVersion();
}

void MQWebApplication::displayVersion()
{
	std::cout << MQWEB_COMPLETE_VERSION << std::endl;
}

int MQWebApplication::main(const std::vector<std::string>& args)
{
	if (_helpRequested)
	{
		displayHelp();
		return Application::EXIT_OK;
	}

	if (_versionRequested)
	{
		displayVersion();
		return Application::EXIT_OK;
	}

	Poco::Logger& logger = Poco::Logger::get("mq.web");

	MQ::MQSubsystem& mqSystem = getSubsystem<MQ::MQSubsystem>();

	try
	{
		mqSystem.load();
	}
	catch(Poco::LibraryLoadException& lle)
	{
		Poco::Logger::get("mq").fatal(lle.message());
		return Application::EXIT_SOFTWARE;
	}

	_cache.setLogger(logger);

	// Check the web app path configuration (mq.web.app)
	if ( config().has("mq.web.app") )
	{
		Poco::Path appPath;
		std::string appValue;
		try
		{
			appValue = config().getString("mq.web.app");
			appValue = config().expand(appValue);
			appPath.assign(appValue);
			appPath.makeDirectory();
		}
		catch(Poco::PathSyntaxException&)
		{
			poco_fatal_f1(logger, "Invalid path specified for mq.web.app: %s", appValue);
			return Application::EXIT_CONFIG;
		}

		Poco::File file = Poco::File(appPath);
		if ( !file.exists() )
		{
			poco_fatal_f1(logger, "Path for static files %s doesn't exist! Check the configuration file.", appPath.toString());
			return Application::EXIT_CONFIG;
		}
	}

	unsigned short port = (unsigned short) config().getInt("mq.web.port", 8081);

	// set-up a server socket
	ServerSocket svs(port);
	// set-up a HTTPServer instance
	HTTPServer srv(new MQ::Web::RequestHandlerFactory, svs, new HTTPServerParams);
	// start the HTTPServer
	srv.start();

	poco_information_f1(logger, "MQWeb started and listening on port %hu", port);

	// wait for CTRL-C or kill
	waitForTerminationRequest();
	// Stop the HTTPServer
	srv.stop();

	MQ::Web::WebSocketRequestHandler::cancel();

	_qmgrPoolCache.clear();

	return Application::EXIT_OK;
}


POCO_SERVER_MAIN(MQWebApplication)
