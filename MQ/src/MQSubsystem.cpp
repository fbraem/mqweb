/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
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
#include <iostream>

#include "MQ/MQSubsystem.h"
#include "Poco/Util/Application.h"

namespace MQ
{

MQSubsystem::MQSubsystem() : Poco::Util::Subsystem(), _connectionMode(BINDING_MODE)
{
}

MQSubsystem::~MQSubsystem()
{
}

void MQSubsystem::initialize(Poco::Util::Application& app)
{
}

void MQSubsystem::uninitialize()
{
}

void MQSubsystem::load()
{
	static std::string windowsPrefix("Windows");
	std::string mode = Poco::Util::Application::instance().config().getString("mq.mode", "bindings");
	_connectionMode = mode.compare("bindings") == 0 ? BINDING_MODE : CLIENT_MODE;

	std::string osName = Poco::Util::Application::instance().config().getString("system.osName");
	std::string library;

	if ( binding() )
	{
		if ( osName.compare(0, windowsPrefix.size(), windowsPrefix) )
		{
			library = "libmqm_r.so";
		}
		else // We assume windows for now
		{
			library = "MQM";
		}
	}
	else
	{
		if ( osName.compare(0, windowsPrefix.size(), windowsPrefix) )
		{
			library = "libmqic_r.so";
		}
		else // We assume windows for now
		{
			library = "MQIC";
		}
	}

	Poco::Logger& logger = Poco::Logger::get("mq");
	poco_information_f3(logger, "Running in %s mode on a %s OS. Trying to load %s", mode, osName, library);
	_functions.loadLibrary(library);
	poco_information_f1(logger, "Library %s loaded", library);
}

} // Namespace MQWeb
