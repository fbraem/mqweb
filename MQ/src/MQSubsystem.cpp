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
