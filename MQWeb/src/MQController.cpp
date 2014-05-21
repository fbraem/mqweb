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
#include "Poco/Util/Application.h"
#include "Poco/JSON/Object.h"

#include "MQ/MQSubsystem.h"
#include "MQ/MQException.h"

#include "MQ/Web/MQController.h"
#include "MQ/Web/MQMapper.h"
#include "MQ/Web/TemplateView.h"
#include "MQ/Web/JSONView.h"

namespace MQ {
namespace Web {


MQController::MQController() : Controller(), _mqwebData(new Poco::JSON::Object())
{
	set("mqweb", _mqwebData);
}


MQController::~MQController()
{
}


void MQController::beforeAction()
{
	_stopwatch.start();

	Poco::JSON::Object::Ptr date = new Poco::JSON::Object();
	_mqwebData->set("date", date);
	date->set("start", Poco::DateTimeFormatter::format(Poco::Timestamp(), Poco::DateTimeFormat::HTTP_FORMAT));

	MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();

	const std::vector<std::string>& parameters = getParameters();

	_mqwebData->set("client", mqSystem.client());

	if ( config.hasProperty("mq.web.qmgr") )
	{
		// When a queuemanager is passed on the command line, we always
		// connect to this queuemanager. When the user specified another
		// queuemanager on the URL, it will be ignored.
		_qmgr = new QueueManager(config.getString("mq.web.qmgr"));
	}
	else
	{
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
	}

	if ( mqSystem.binding() )
	{
		_qmgr->connect();
	}
	else
	{
		// In client mode we check for a configuration
		// When this is not available, we hope that a channel tab file
		// is configured.
		std::string qmgrConfig = "mq.web.qmgr." + _qmgr->name();
		std::string qmgrConfigConnection = qmgrConfig + ".connection";
		if ( config.has(qmgrConfigConnection) )
		{
			std::string connection;
			std::string channel;
			connection = config.getString(qmgrConfigConnection);
			std::string qmgrConfigChannel = qmgrConfig + ".channel";
			if ( config.has(qmgrConfigChannel) )
			{
				channel = config.getString(qmgrConfigChannel);
			}
			else
			{
				channel = config.getString("mq.web.defaultChannel", "SYSTEM.DEF.SVRCONN");
			}
			if ( config.has("mq.web.ssl.keyrepos") )
			{
				Poco::AutoPtr<Poco::Util::AbstractConfiguration> sslConfig = config.createView("mq.web.ssl");
				_qmgr->connect(channel, connection, *sslConfig.get());
			}
			else
			{
				_qmgr->connect(channel, connection);
			}
		}
		else // Hope that there is a channel tab file available
		{
			_qmgr->connect();
		}
	}

	_mqwebData->set("qmgr", _qmgr->name());
	_mqwebData->set("zos", _qmgr->zos());
	_mqwebData->set("qmgrId", _qmgr->id());

	std::string qmgrConfig = "mq.web.qmgr." + _qmgr->name();
	std::string qmgrConfigModel = qmgrConfig + ".reply";
	
	std::string modelQ;
	if ( config.has(qmgrConfigModel) )
	{
		modelQ = config.getString(qmgrConfigModel);
	}
	else
	{
		modelQ = config.getString("mq.web.reply", "SYSTEM.DEFAULT.MODEL.QUEUE");
	}
	_mqwebData->set("replyq", modelQ);
	_mqwebData->set("cmdq", _qmgr->commandQueue());

	_commandServer = new CommandServer(_qmgr, modelQ);
}


void MQController::handleException(const MQException& mqe)
{
	Poco::JSON::Object::Ptr error = new Poco::JSON::Object();

	set("error", error);
	error->set("object", mqe.object());
	error->set("fn", mqe.function());
	switch(mqe.code())
	{
		case MQCC_OK: error->set("code", "OK"); break;
		case MQCC_WARNING: error->set("code", "WARNING"); break;
		case MQCC_FAILED: error->set("code", "ERROR"); break;
	}

	Poco::JSON::Object::Ptr reason = new Poco::JSON::Object();
	error->set("reason", reason);
	reason->set("code", mqe.reason());
	reason->set("desc", MQMapper::getReasonString(mqe.reason()));

	if ( isJSON() )
	{
		setView(new JSONView());
	}
	else
	{
		setView(new TemplateView("error.tpl"));
	}

}


void MQController::afterAction()
{
	Poco::JSON::Object::Ptr date = _mqwebData->getObject("date");
	if ( ! date.isNull() )
	{
		date->set("end", Poco::DateTimeFormatter::format(Poco::Timestamp(), Poco::DateTimeFormat::HTTP_FORMAT));
	}
	
	_stopwatch.stop();
	_mqwebData->set("elapsed", (double) _stopwatch.elapsed() / 1000000 );

	Controller::afterAction();
}


void MQController::handle(const std::vector<std::string>& parameters, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	try
	{
		Controller::handle(parameters, request, response);
	}
	catch(MQException& mqe)
	{
		handleException(mqe);
		render();
	}
	catch(...)
	{
		//TODO: redirect to an error page
	}
}


}} // namespace MQ::Web
