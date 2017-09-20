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
#include "MQ/Web/ControllerRequestHandler.h"
#include "MQ/Web/AuthenticationInformationController.h"
#include "MQ/Web/AuthorityRecordController.h"
#include "MQ/Web/AuthorityServiceController.h"
#include "MQ/Web/ChannelController.h"
#include "MQ/Web/ChannelInitiatorController.h"
#include "MQ/Web/ChannelStatusController.h"
#include "MQ/Web/ClusterQueueManagerController.h"
#include "MQ/Web/ConnectionController.h"
#include "MQ/Web/ListenerController.h"
#include "MQ/Web/ListenerStatusController.h"
#include "MQ/Web/MessageController.h"
#include "MQ/Web/MQWebController.h"
#include "MQ/Web/NamelistController.h"
#include "MQ/Web/ProcessController.h"
#include "MQ/Web/QueueController.h"
#include "MQ/Web/QueueManagerController.h"
#include "MQ/Web/QueueManagerStatusController.h"
#include "MQ/Web/QueueStatusController.h"
#include "MQ/Web/ServiceController.h"
#include "MQ/Web/SubscriptionController.h"
#include "MQ/Web/SubscriptionStatusController.h"
#include "MQ/Web/TopicController.h"
#include "MQ/Web/TopicStatusController.h"

#include "Poco/URI.h"

namespace MQ {
namespace Web {


Poco::DynamicFactory<Controller> ControllerRequestHandler::_controllerFactory;


void ControllerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	Poco::Logger& logger = Poco::Logger::get("mq.web");

	Poco::URI uri(request.getURI());

	std::vector<std::string> paths;
	uri.getPathSegments(paths);

	if ( paths.size() == 0 ) {
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Invalid path specified in URL. Did you specified api?");
		response.send();
		return;
	}

	std::string controllerType = paths.front();
	paths.erase(paths.begin());

	std::string controllerName;

	Poco::SharedPtr<Controller> controller;
	if ( controllerType.compare("api") == 0 )
	{
		if ( paths.size() == 0 )
		{
			controllerName = "qmgr";
		}
		else
		{
			controllerName = paths.front();
			paths.erase(paths.begin());
		}
		poco_trace_f1(Poco::Logger::get("mq.web"), "api controller: %s", controllerName);
		try
		{
			Poco::SharedPtr<Controller> controller = _controllerFactory.createInstance(controllerName);
			controller->handle(paths, request, response);
		}
		catch(Poco::NotFoundException&)
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND, "Unknown controller");
			response.send();
		}
	}
	else
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Only api is allowed as controller type");
		response.send();
	}
}


void ControllerRequestHandler::registerControllers()
{
	_controllerFactory.registerClass<QueueManagerController>("qmgr");
	_controllerFactory.registerClass<QueueManagerStatusController>("qmstatus");
	_controllerFactory.registerClass<QueueController>("queue");
	_controllerFactory.registerClass<ChannelController>("channel");
	_controllerFactory.registerClass<MessageController>("message");
	_controllerFactory.registerClass<ChannelStatusController>("chstatus");
	_controllerFactory.registerClass<ListenerController>("listener");
	_controllerFactory.registerClass<ListenerStatusController>("lsstatus");
	_controllerFactory.registerClass<ClusterQueueManagerController>("clusqmgr");
	_controllerFactory.registerClass<ConnectionController>("conn");
	_controllerFactory.registerClass<NamelistController>("nl");
	_controllerFactory.registerClass<ProcessController>("process");
	_controllerFactory.registerClass<ServiceController>("service");
	_controllerFactory.registerClass<QueueStatusController>("qstatus");
	_controllerFactory.registerClass<AuthenticationInformationController>("authinfo");
	_controllerFactory.registerClass<AuthorityRecordController>("authrec");
	_controllerFactory.registerClass<SubscriptionController>("sub");
	_controllerFactory.registerClass<SubscriptionStatusController>("sbstatus");
	_controllerFactory.registerClass<TopicController>("topic");
	_controllerFactory.registerClass<TopicStatusController>("tpstatus");
	_controllerFactory.registerClass<AuthorityServiceController>("authservice");
	_controllerFactory.registerClass<ChannelInitiatorController>("chinit");
	_controllerFactory.registerClass<MQWebController>("mqweb");
}


} } // Namespace MQ::Web
