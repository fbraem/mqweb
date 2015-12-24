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
	set("mqweb", mqweb);

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
	set("queuemanagers", queuemanagers);

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
	else
	{
	}
}

}} // Namespace MQ::Web

