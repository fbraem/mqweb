/*
* Copyright 2017 - KBC Group NV - Franky Braem - The MIT license
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "MQ/MQSubsystem.h"
#include "MQ/Web/Version.h"
#include "MQ/Web/MQWebSubsystem.h"
#include "MQ/Web/Application.h"
#include "MQ/Web/RequestHandlerFactory.h"
#include "MQ/Web/WebSocketRequestHandler.h"
#include "MQ/Web/QueueManagerDatabaseConfig.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"

#include "Poco/Data/SQLite/Connector.h"
#ifdef MQWEB_ODBC_CONFIG
	#include "Poco/Data/ODBC/Connector.h"
#endif

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
	addSubsystem(new MQ::Web::MQWebSubsystem());
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
#ifdef MQWEB_ODBC_CONFIG
	Poco::Data::ODBC::Connector::registerConnector();
#endif

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
#ifdef MQWEB_ODBC_CONFIG
	Poco::Data::ODBC::Connector::unregisterConnector();
#endif

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

	if (config().has("mq.web.config.connection"))
	{
		std::string dbConnector = config().getString("mq.web.config.connector", Poco::Data::SQLite::Connector::KEY);
		std::string dbConnection = config().getString("mq.web.config.connection");

		Poco::SharedPtr<MQ::Web::QueueManagerDatabaseConfig> qmgrConfig;
		if (config().hasProperty("mq.web.config.tablename"))
		{
			std::string tableName = config().getString("mq.web.config.tablename");
			qmgrConfig = new MQ::Web::QueueManagerDatabaseConfig(dbConnector, dbConnection, tableName);
		}
		else
		{
			qmgrConfig = new MQ::Web::QueueManagerDatabaseConfig(dbConnector, dbConnection);
		}
		std::map<std::string, Poco::DynamicStruct> qmgrs = qmgrConfig->read();
		for (std::map<std::string, Poco::DynamicStruct>::iterator it = qmgrs.begin(); it != qmgrs.end(); ++it) 
		{
			config().setString("mq.web.qmgr." + it->first + ".channel", it->second["channel"]);
			config().setString("mq.web.qmgr." + it->first + ".connection", it->second["connection"]);
		}
	}

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

	Poco::ThreadPool::defaultPool().addCapacity(100);

	unsigned short port = (unsigned short) config().getInt("mq.web.port", 8081);

	// set-up a server socket
	ServerSocket svs(port);
	// set-up a HTTPServer instance
	HTTPServer srv(new MQ::Web::RequestHandlerFactory, svs, new HTTPServerParams);
	// start the HTTPServer
	srv.start();

	logger.information("MQWeb started and listening on port %hu", port);

	// wait for CTRL-C or kill
	waitForTerminationRequest();
	// Stop the HTTPServer
	srv.stop();

	logger.trace("Application stopping ...");

	return Application::EXIT_OK;
}


POCO_SERVER_MAIN(MQWebApplication)
