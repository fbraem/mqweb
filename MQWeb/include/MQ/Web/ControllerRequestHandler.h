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

#ifndef _MQWeb_ControllerRequestHandler_INCLUDED
#define _MQWeb_ControllerRequestHandler_INCLUDED

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/DynamicFactory.h"

#include "MQ/Web/Controller.h"

namespace MQ {
namespace Web {

class ControllerRequestHandler: public Poco::Net::HTTPRequestHandler
{
	/// Class for creating a controller based on the request.
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		/// Handles the request by creating the controller and calling the handle method.

	static void registerControllers();
		/// Register all available controllers.

private:

	static Poco::DynamicFactory<Controller> _controllerFactory;
};


} } // Namespace MQ::Web

#endif // _MQWeb_ControllerRequestHandler_INCLUDED
