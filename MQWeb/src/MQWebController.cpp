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
#include "Poco/JSON/Object.h"
#include "Poco/Util/Application.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/LineEndingConverter.h"
#include "Poco/RegularExpression.h"
#include "Poco/Logger.h"

#include "Poco/Data/SQLite/Connector.h"

#include "MQ/Web/MQWebController.h"
#include "MQ/Web/Version.h"
#include "MQ/MQSubsystem.h"
#include "MQ/Web/QueueManagerDefaultConfig.h"
#include "MQ/Web/QueueManagerDatabaseConfig.h"
#include "MQ/Web/QueueManagerPoolCache.h"

namespace MQ
{
namespace Web
{


MQWebController::MQWebController() : Controller()
{
}


MQWebController::~MQWebController()
{
}


void MQWebController::inquire()
{
	Poco::JSON::Object::Ptr mqweb = new Poco::JSON::Object();
	setData("data", mqweb);

	Poco::JSON::Object::Ptr version = new Poco::JSON::Object();
	mqweb->set("version", version);

	version->set("major", MQWEB_VERSION_MAJOR);
	version->set("minor", MQWEB_VERSION_MINOR);
	version->set("release", MQWEB_RELEASE_NUMBER);
	version->set("short", MQWEB_VERSION);
	version->set("complete", MQWEB_COMPLETE_VERSION);

	Poco::JSON::Object::Ptr date = new Poco::JSON::Object();
	mqweb->set("date", date);
	date->set("start", Poco::DateTimeFormatter::format(Poco::Timestamp(), Poco::DateTimeFormat::HTTP_FORMAT));

	MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();
	mqweb->set("client", mqSystem.client());

	Poco::JSON::Array::Ptr connections = new Poco::JSON::Array();
	mqweb->set("connections", connections);
	std::set<std::string> pools = QueueManagerPoolCache::instance()->getAllPoolNames();
	for(std::set<std::string>::const_iterator it = pools.begin(); it != pools.end(); ++it)
	{
		QueueManagerPool::Ptr pool = QueueManagerPoolCache::instance()->getQueueManagerPool(*it);
		Poco::JSON::Object::Ptr jsonPool = new Poco::JSON::Object();
		connections->add(jsonPool);
		jsonPool->set("name", *it);
		jsonPool->set("size", pool->size());
		jsonPool->set("capacity", pool->capacity());
		jsonPool->set("peakCapacity", pool->peakCapacity());
		jsonPool->set("available", pool->available());
	}
}

void MQWebController::list()
{
	Poco::JSON::Array::Ptr queuemanagers = new Poco::JSON::Array();
	setData("data", queuemanagers);

	MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();
	if ( mqSystem.client() )
	{
		std::vector<std::string> configuredQueuemanagers;
		QueueManagerDefaultConfig qmgrConfig(config);
		qmgrConfig.list(configuredQueuemanagers);
		for(std::vector<std::string>::iterator it = configuredQueuemanagers.begin(); it != configuredQueuemanagers.end(); ++it)
		{
			queuemanagers->add(*it);
		}
	}
	else // Binding mode, try dspmq to get the list of installed queuemanagers
	{
		Poco::RegularExpression re("QMNAME\\((.*)\\)\\s*STATUS\\((.*)\\)");

		Poco::Process::Args args;
		args.push_back("-n");
		args.push_back("-s");
		Poco::Pipe outPipe;

		try
		{
			Poco::Util::Application& app = Poco::Util::Application::instance();
			std::string mqBinPathname = app.config().getString("mq.bin", "");
			std::string command = mqBinPathname + "/dspmq";

			Poco::ProcessHandle ph = Poco::Process::launch(command, args, 0, &outPipe, 0);

			Poco::PipeInputStream istr(outPipe);

			std::string line;
			int lc = 0;
			while(std::getline(istr, line))
			{
				Poco::RegularExpression::MatchVec match;
				if ( re.match(line, 0, match) == 3 )
				{
					queuemanagers->add(line.substr(match[1].offset, match[1].length));
				}
			}
			int rc = ph.wait();
			//assert (rc == 1);
		}
		catch(Poco::SystemException& e)
		{
			Poco::Logger& logger = Poco::Logger::get("mq.web");
			poco_error(logger, e.displayText());
		}
	}
}

}} // Namespace MQ::Web
