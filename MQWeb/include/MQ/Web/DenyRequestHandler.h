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
#ifndef _MQWeb_DenyRequestHandler_H
#define _MQWeb_DenyRequestHandler_H

#include <Poco/Net/HTTPRequestHandler.h>

namespace MQ {
namespace Web {

class DenyRequestHandler: public Poco::Net::HTTPRequestHandler
	/// Implements a request handler that returns a HTTP FORBIDDEN status
{
public:

	DenyRequestHandler();
		/// Constructor

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		/// Sets the HTTP FORBIDDEN status and logs the IP address that is denied.
};

} } // Namespace MQ::Web


#endif //_MQWeb_DenyRequestHandler_H
