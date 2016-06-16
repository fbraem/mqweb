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
#include "Poco/Dynamic/Struct.h"
#include "Poco/JSON/Object.h"

#include "MQ/MQSubsystem.h"
#include "MQ/MQException.h"

#include "MQ/Web/MQController.h"
#include "MQ/Web/MQMapper.h"

namespace MQ {
namespace Web {


MQController::MQController() : Controller(), _meta(new Poco::JSON::Object()), _commandServer(NULL)
{
	set("meta", _meta);
}


MQController::~MQController()
{
}


void MQController::beforeAction()
{
	_stopwatch.start();

	Poco::JSON::Object::Ptr date = new Poco::JSON::Object();
	_meta->set("date", date);
	date->set("start", Poco::DateTimeFormatter::format(Poco::Timestamp(), Poco::DateTimeFormat::HTTP_FORMAT));

	MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();

	_meta->set("client", mqSystem.client());

	std::string qmgrName;
	if ( config.hasProperty("mq.web.qmgr") )
	{
		// When a queuemanager is passed on the command line, we always
		// connect to this queuemanager. When the user specified another
		// queuemanager on the URL, it will be ignored.
		qmgrName = config.getString("mq.web.qmgr");
	}
	else
	{
		const std::vector<std::string>& parameters = getParameters();
		if ( parameters.size() > 0 )
		{
			qmgrName = parameters[0];
		}
		else if ( mqSystem.client() )
		{
			qmgrName = config.getString("mq.web.defaultQmgr", "*");
		}
	}

	Poco::SharedPtr<QueueManagerPool> qmgrPool = QueueManagerPoolCache::instance()->getQueueManagerPool(qmgrName);
	if ( qmgrPool.isNull() )
	{
		setResponseStatus(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, "Out of memory: can't create a pool for queuemanager.");
		return;
	}

	QueueManager::Ptr qmgr = qmgrPool->borrowObject();
	if ( qmgr.isNull() )
	{
		setResponseStatus(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, "No queuemanager available in the pool. Check the connection pool configuration.");
		return;
	}
	_qmgrPoolGuard = new QueueManagerPoolGuard(qmgrPool, qmgr);

	_meta->set("qmgr", qmgr->name());
	_meta->set("zos", qmgr->zos());
	_meta->set("qmgrId", qmgr->id());

	_commandServer = qmgr->commandServer();
	if ( _commandServer == NULL )
	{
		std::string qmgrConfigReplyQ = "mq.web.qmgr." + qmgrName + ".reply";

		std::string replyQ;
		if ( config.has(qmgrConfigReplyQ) )
		{
			replyQ = config.getString(qmgrConfigReplyQ);
		}
		else
		{
			replyQ = config.getString("mq.web.reply", "SYSTEM.DEFAULT.MODEL.QUEUE");
		}
		_commandServer = qmgr->createCommandServer(replyQ);
	}

	if ( _commandServer != NULL )
	{
		_meta->set("replyq", _commandServer->replyQName());
		_meta->set("cmdq", _commandServer->commandQName());
	}
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
}


void MQController::afterAction()
{
	Poco::JSON::Object::Ptr date = _meta->getObject("date");
	if ( ! date.isNull() )
	{
		date->set("end", Poco::DateTimeFormatter::format(Poco::Timestamp(), Poco::DateTimeFormat::HTTP_FORMAT));
	}
	
	_stopwatch.stop();
	_meta->set("elapsed", (double) _stopwatch.elapsed() / 1000000 );

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
		afterAction();
	}
	catch(...)
	{
		//TODO: redirect to an error page
	}
}

void MQController::handleFilterForm(Poco::JSON::Object::Ptr pcfParameters)
{
	if ( form().has("Filter") && form().has("FilterParam") && form().has("FilterValue") )
	{
		Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();
		filter->set("Parameter", form().get("FilterParam"));
		filter->set("Operator", form().get("FilterOp", "EQ"));
		filter->set("FilterValue", form().get("FilterVlue"));

		std::string filterType = form().get("Filter");
		if ( Poco::icompare(filterType, "I") == 0 )
		{
			pcfParameters->set("IntegerFilterCommand", filter);
		}
		else if ( Poco::icompare(filterType, "S") == 0 )
		{
			pcfParameters->set("StringFilterCommand", filter);
		}
	}
}

}} // namespace MQ::Web
