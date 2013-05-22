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

#ifndef _MQWeb_RequestHandlerFactory_H
#define _MQWeb_RequestHandlerFactory_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

namespace MQ
{
namespace Web
{

class RequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	/// Our factory for creating a HTTPRequestHandler derived class
{
public:

	RequestHandlerFactory();
		/// Constructor

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
		/// Creates a HTTPRequestHandler
private:

	bool filter(const Poco::Net::HTTPServerRequest& request);
		/// Checks if a connection is allowed based on the incoming IP address.
		/// First all mq.web.allow properties are checked. When the IP is not allowed, the
		/// DenyRequestHandler will be returned. Then all mq.web.deny properties are checked.
		/// When the IP must be denied, the DenyRequestHandler is also returned.
		/// By default the request is allowed and will try to create a HTTPRequestHandler.
};

} } // Namespace MQ::Web

#endif //_MQWeb_RequestHandlerFactory_H
