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
#include "MQ/Web/RequestHandlerFactory.h"
#include "MQ/Web/AppRequestHandler.h"
#include "MQ/Web/DenyRequestHandler.h"
#include "MQ/Web/WebSocketRequestHandler.h"
#include "MQ/Web/ControllerRequestHandler.h"

#include "Poco/Logger.h"
#include "Poco/RegularExpression.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/AbstractConfiguration.h"

namespace MQ
{
namespace Web
{

RequestHandlerFactory::RequestHandlerFactory() : Poco::Net::HTTPRequestHandlerFactory()
{
	ControllerRequestHandler::registerControllers();
}

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
	static std::string appURI = "/web";
	std::string uri = request.getURI();

	Poco::Logger& logger = Poco::Logger::get("mq.web.access");
	logger.information(Poco::Logger::format("IP: $0 URI: $1 ($2)", request.clientAddress().toString(), uri, request.getMethod()));

	if ( ! filter(request) )
	{
		return new DenyRequestHandler();
	}

	// Check for a websocket ...
	if ( request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0 )
	{
		return new WebSocketRequestHandler();
	}

	if ( ! uri.compare(0, appURI.size(), appURI)
		|| ! uri.compare("/favicon.ico") )
	{
		return new AppRequestHandler();
	}

	return new ControllerRequestHandler();
}

bool RequestHandlerFactory::filter(const Poco::Net::HTTPServerRequest& request)
{
	Poco::Logger& logger = Poco::Logger::get("mq.web");

	bool allowed = true;

	std::string ip = request.clientAddress().host().toString();

	//TODO: move this to Application and exit MQWeb when an invalid regex is found
	static Poco::Util::AbstractConfiguration* allowIPs = Poco::Util::Application::instance().config().createView("mq.web.allow");
	static Poco::Util::AbstractConfiguration* denyIPs = Poco::Util::Application::instance().config().createView("mq.web.deny");

	Poco::Util::AbstractConfiguration::Keys keys;

	allowIPs->keys(keys);
	if ( keys.size() > 0 ) // Check if IP is allowed
	{
		allowed = false;
		for(Poco::Util::AbstractConfiguration::Keys::iterator it = keys.begin(); it != keys.end(); ++it)
		{
			std::string regexValue = allowIPs->getString(*it);
			poco_trace_f3(logger, "IP Allow Check %s : %s (%s)", ip, regexValue, *it);
			Poco::RegularExpression regex(allowIPs->getString(*it));
			if ( regex.match(ip) )
			{
				poco_debug_f2(logger, "IP %s allowed (mq.web.allow.%s matched)", ip, *it);
				allowed = true;
				break;
			}
		}
	}

	denyIPs->keys(keys);
	if ( keys.size() > 0 ) // Check if IP is denied
	{
		for(Poco::Util::AbstractConfiguration::Keys::iterator it = keys.begin(); it != keys.end(); ++it)
		{
			std::string regexValue = denyIPs->getString(*it);

			poco_trace_f3(logger, "IP Deny Check %s : %s (%s)", ip, regexValue, *it);
			Poco::RegularExpression regex(denyIPs->getString(*it));
			if ( regex.match(ip) )
			{
				poco_warning_f2(logger, "IP %s denied (mq.web.deny.%s matched)", ip, *it);
				allowed = false;
				break;
			}
		}
	}

	return allowed;
}

} } // Namespace MQ::Web
