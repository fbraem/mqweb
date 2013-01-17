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
#include <MQ/MQSubsystem.h>
#include <MQ/Web/Application.h>
#include <MQ/Web/RequestHandlerFactory.h>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerParams.h>

#include <Poco/Logger.h>
#include <Poco/File.h>

#include <iostream>

using namespace Poco::Net;
using namespace Poco::Util;


MQWebApplication::MQWebApplication(): _helpRequested(false)
{
	addSubsystem(new MQ::MQSubsystem());
}

MQWebApplication::~MQWebApplication()
{
}

void MQWebApplication::initialize(Application& self)
{
	int count = loadConfiguration(); // load default configuration files, if present
	try
	{
		ServerApplication::initialize(self);
	}
	catch(Poco::NotFoundException nfe)
	{
	// We can get here when some log configuration is not available
		std::cout << "Please check your logger configuration because we got a NotFoundException for " << nfe.message() << std::endl;
	}
}

void MQWebApplication::uninitialize()
{
	Poco::Logger::get("mq.web").information("MQWeb process stopped!");

	ServerApplication::uninitialize();
}

void MQWebApplication::defineOptions(Poco::Util::OptionSet& options)
{
	ServerApplication::defineOptions(options);

	options.addOption(
	Option("help", "h", "display help information on command line arguments")
		.required(false)
		.repeatable(false));
}

void MQWebApplication::handleOption(const std::string& name, const std::string& value)
{
	ServerApplication::handleOption(name, value);

	if (name == "help")
	{
		_helpRequested = true;
	}
}

void MQWebApplication::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("A web server for managing Websphere MQ.");
	helpFormatter.format(std::cout);
}

int MQWebApplication::main(const std::vector<std::string>& args)
{
	if (_helpRequested)
	{
		displayHelp();
		return Application::EXIT_OK;
	}

	Poco::Logger& logger = Poco::Logger::get("mq.web");

	// For now on, load in binding mode
	MQ::MQSubsystem& mqSystem = getSubsystem<MQ::MQSubsystem>();

	try
	{
		mqSystem.load();
	}
	catch(Poco::LibraryLoadException lle)
	{
		Poco::Logger::get("mq").fatal(lle.message());
		return Application::EXIT_SOFTWARE;
	}

	_cache.setLogger(logger);

	// Check the template path configuration (mq.web.templates)
	Poco::Path templatePath;
	std::string templatesValue;
	try
	{
		templatesValue = config().getString("mq.web.templates");
		templatesValue = config().expand(templatesValue);
		templatePath.assign(templatesValue);
		templatePath.makeDirectory();
	}
	catch(Poco::NotFoundException nfe)
	{
		templatePath.assign(config().getString("application.dir"));
		templatePath.pushDirectory("templates");
		poco_warning_f1(logger, "No mq.web.templates found in the configuration file. Trying to use %s", templatePath.toString());
	}
	catch(Poco::PathSyntaxException pse)
	{
		poco_fatal_f1(logger, "Invalid path specified for mq.web.templates: %s", templatesValue);
		return Application::EXIT_CONFIG;
	}

	Poco::File file(templatePath);
	if ( !file.exists() )
	{
		poco_fatal_f1(logger, "Template path %s doesn't exist! Check the configuration file.", templatePath.toString());
		return Application::EXIT_CONFIG;
	}

	_cache.addPath(templatePath);

	// Check the static path configuration (mq.web.static)
	Poco::Path staticPath;
	std::string staticValue;
	try
	{
		staticValue = config().getString("mq.web.static");
		staticValue = config().expand(staticValue);
		staticPath.assign(staticValue);
		staticPath.makeDirectory();
	}
	catch(Poco::NotFoundException nfe)
	{
		staticPath.assign(config().getString("application.dir"));
		staticPath.pushDirectory("static");
		config().setString("mq.web.static", staticPath.toString());
		poco_warning_f1(logger, "No mq.web.static found in the configuration file. Trying to use %s", staticPath.toString());
	}
	catch(Poco::PathSyntaxException pse)
	{
		poco_fatal_f1(logger, "Invalid path specified for mq.web.static: %s", staticValue);
		return Application::EXIT_CONFIG;
	}

	file = Poco::File(staticPath);
	if ( !file.exists() )
	{
		poco_fatal_f1(logger, "Path for static files %s doesn't exist! Check the configuration file.", staticPath.toString());
		return Application::EXIT_CONFIG;
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

	return Application::EXIT_OK;
}

POCO_SERVER_MAIN(MQWebApplication)
