/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
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
#include "MQ/Web/QueueController.h"
#include "MQ/Web/QueueManagerController.h"
#include "MQ/Web/QueueManagerStatusController.h"
#include "MQ/Web/ChannelController.h"
#include "MQ/Web/ChannelStatusController.h"
#include "MQ/Web/MessageController.h"

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

	std::string controllerName;
	if ( paths.size() == 0 ) // Use the default controller
	{
		controllerName = "qmgr";
	}
	else
	{
		controllerName = paths.front();
		paths.erase(paths.begin());
	}

	poco_trace_f1(Poco::Logger::get("mq.web"), "MQ Controller: %s", controllerName);
	try
	{
		Poco::SharedPtr<Controller> controller = _controllerFactory.createInstance(controllerName);
		if ( controller.isNull() )
		{
			//TODO: redirect to error page!
		}
		controller->handle(paths, request, response);
	}
	catch(Poco::NotFoundException&)
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND, "Unknown controller");
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
}


} } // Namespace MQ::Web
