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
#include "Poco/Util/Application.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/JSON/Object.h"

#include "MQ/MQSubsystem.h"
#include "MQ/MQException.h"

#include "MQ/Web/MQController.h"
#include "MQ/Web/PCFCommand.h"

namespace MQ {
namespace Web {


MQController::MQController() : Controller(), _meta(new Poco::JSON::Object()), _commandServer(NULL)
{
	setData("meta", _meta);
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
		setResponseStatus(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, "Can't create a pool for queuemanager. Check your configuration or memory limitations.");
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
}

Poco::SharedPtr<CommandServer> MQController::commandServer()
{
	if (_commandServer.isNull())
	{
		MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
		Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();

		QueueManager::Ptr qmgr = _qmgrPoolGuard->getObject();
		std::string qmgrConfigReplyQ = "mq.web.qmgr." + qmgr->name() + ".reply";

		std::string replyQ;
		if (config.has(qmgrConfigReplyQ))
		{
			replyQ = config.getString(qmgrConfigReplyQ);
		}
		else
		{
			replyQ = config.getString("mq.web.reply", "SYSTEM.DEFAULT.MODEL.QUEUE");
		}
		_commandServer = new CommandServer(qmgr, replyQ);
		_meta->set("replyq", _commandServer->replyQName());
		_meta->set("cmdq", _commandServer->commandQName());
	}

	return _commandServer;
}


void MQController::handleException(const MQException& mqe)
{
	Poco::JSON::Object::Ptr error = new Poco::JSON::Object();

	setData("error", error);
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
	reason->set("desc", PCFCommand::getReasonString(mqe.reason()));
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
