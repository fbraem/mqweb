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
#include <Poco/Util/Application.h>
#include <Poco/JSON/Object.h>

#include "MQ/Web/MQController.h"
#include "MQ/MQSubsystem.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {


MQController::MQController() : Controller()
{
}


MQController::~MQController()
{
}


void MQController::beforeAction()
{
	MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();

	const std::vector<std::string>& parameters = getParameters();

	if ( parameters.size() > 0 )
	{
	 _qmgr = new QueueManager(parameters[0]);
	}
	else
	{
		if ( mqSystem.client() )
		{
			_qmgr = new QueueManager(config.getString("mq.web.defaultQmgr", "*"));
		}
		else // In bindings mode we can connect to the default queuemanager
		{
			_qmgr = new QueueManager();
		}
	}

	bool connected = false;
	if ( mqSystem.binding() )
	{
		connected = connect();
	}
	else
	{
		// In client mode we check for a configuration
		// When this is not available, we hope that a channel tab file
		// is configured.
		std::string qmgrConfig = "mq.web.qmgr." + _qmgr->name();
		std::string qmgrConfigConnection = qmgrConfig + ".connection";
		std::string qmgrConfigChannel = qmgrConfig + ".channel";

		if ( config.has(qmgrConfigConnection) )
		{
			std::string connection;
			std::string channel;

			try
			{
				connection = config.getString(qmgrConfigConnection);
			}
			catch(Poco::NotFoundException&)
			{
				poco_error_f1(Poco::Logger::get("mq.web"), "Can't find %s property in configuration file", qmgrConfigConnection);
				//TODO: redirect to error page
			}

			try
			{
				channel = config.getString(qmgrConfigChannel);
			}
			catch(Poco::NotFoundException&)
			{
				poco_error_f1(Poco::Logger::get("mq.web"), "Can't find %s property in configuration file", qmgrConfigChannel);
				//TODO: redirect to error page
			}

			connected = connect(channel, connection);
		}
		else // Hope that there is a channel tab file available
		{
			connected = connect();
		}
	}

	if ( ! connected )
		return;

	Poco::JSON::Object::Ptr jsonQmgr(new Poco::JSON::Object());
	set("qmgr", jsonQmgr);

	jsonQmgr->set("name", _qmgr->name());
	jsonQmgr->set("zos", _qmgr->zos());
	jsonQmgr->set("createDate", Poco::DateTimeFormatter::format(_qmgr->creationDate(), "%d-%m-%Y %H:%M:%S"));
	jsonQmgr->set("alterDate", Poco::DateTimeFormatter::format(_qmgr->alterationDate(), "%d-%m-%Y %H:%M:%S"));
	jsonQmgr->set("id", _qmgr->id());
	jsonQmgr->set("description", _qmgr->description());

	//TODO: configure the model queue!
	_commandServer = new CommandServer(_qmgr, "SYSTEM.MQEXPLORER.REPLY.MODEL");
}


void MQController::handleException(const MQException& mqe)
{
	Poco::JSON::Object::Ptr error = new Poco::JSON::Object();
	set("error", error);
	error->set("object", mqe.object());
	error->set("function", mqe.function());
	switch(mqe.code())
	{
		case MQCC_OK: error->set("code", "OK"); break;
		case MQCC_WARNING: error->set("code", "WARNING"); break;
		case MQCC_FAILED: error->set("code", "ERROR"); break;
	}
	error->set("reason", mqe.reason());
}

bool MQController::connect()
{
	bool ok = false;
	try
	{
		_qmgr->connect();
		ok = true;
	}
	catch(MQException& mqe)
	{
		handleException(mqe);
	}
	return ok;
}

bool MQController::connect(const std::string& channel, const std::string& connection)
{
	bool ok = false;
	try
	{
		_qmgr->connect(channel, connection);
		ok = true;
	}
	catch(MQException& mqe)
	{
		handleException(mqe);
	}
	return ok;
}



}} // namespace MQ::Web
