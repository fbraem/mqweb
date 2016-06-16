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
#include "Poco/JSON/Object.h"
#include "Poco/Util/Application.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/LineEndingConverter.h"
#include "Poco/RegularExpression.h"
#include "Poco/Logger.h"

#include "MQ/Web/MQWebController.h"
#include "MQ/Web/Version.h"
#include "MQ/MQSubsystem.h"

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
	set("data", mqweb);

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
}

void MQWebController::list()
{
	Poco::JSON::Array::Ptr queuemanagers = new Poco::JSON::Array();
	set("data", queuemanagers);

	MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();
	if ( mqSystem.client() )
	{
		Poco::Util::AbstractConfiguration::Keys qmgrs;
		config.keys("mq.web.qmgr", qmgrs);

		for(Poco::Util::AbstractConfiguration::Keys::iterator it = qmgrs.begin(); it != qmgrs.end(); ++it)
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

