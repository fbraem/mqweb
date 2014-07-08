/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
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
#include "MQ/Web/ControllerRequestHandler.h"
#include "MQ/Web/WebController.h"
#include "MQ/Web/QueueController.h"
#include "MQ/Web/QueueManagerController.h"
#include "MQ/Web/QueueManagerStatusController.h"
#include "MQ/Web/ChannelController.h"
#include "MQ/Web/ChannelStatusController.h"
#include "MQ/Web/ConnectionController.h"
#include "MQ/Web/MessageController.h"
#include "MQ/Web/ListenerController.h"
#include "MQ/Web/ListenerStatusController.h"
#include "MQ/Web/ClusterQueueManagerController.h"

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

	std::string controllerType;
	std::string controllerName;
	if ( paths.size() == 0 ) // Use the default controller type
	{
		controllerType = "web";
	}
	else
	{
		controllerType = paths.front();
		paths.erase(paths.begin());
	}

	Poco::SharedPtr<Controller> controller;
	if ( controllerType.compare("web") == 0 )
	{
		controller = new WebController();
		if ( paths.size() == 0 )
		{
			paths.push_back("qmgr"); // Default qmgr
		}
		controller->handle(paths, request, response);
	}
	else if ( controllerType.compare("api") == 0 )
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
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Only web or api are allowed as controller type");
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
}


} } // Namespace MQ::Web
