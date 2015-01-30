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

#ifndef _MQWeb_WebSocketRequestHandler_INCLUDED
#define _MQWeb_WebSocketRequestHandler_INCLUDED

#include "Poco/Net/HTTPRequestHandler.h"

#include "Poco/TaskManager.h"

namespace MQ {
namespace Web {

class WebSocketRequestHandler: public Poco::Net::HTTPRequestHandler
{
	/// Class for creating a controller based on the request.
public:
	WebSocketRequestHandler();

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		/// Handles the request.

	static void cancel();
		/// Cancel all websocket tasks. Should be called before the application ends ...

private:
	static Poco::ThreadPool _tmThreadPool;

	static Poco::TaskManager _tm;

};


} } // Namespace MQ::Web

#endif // _MQWeb_WebSocketRequestHandler_INCLUDED
