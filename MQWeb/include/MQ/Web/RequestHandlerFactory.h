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
#ifndef _MQWeb_RequestHandlerFactory_H
#define _MQWeb_RequestHandlerFactory_H

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"

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
