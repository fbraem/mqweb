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
#include "MQ/Web/RequestHandlerFactory.h"
#include "MQ/Web/StaticRequestHandler.h"
#include "MQ/Web/DenyRequestHandler.h"
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
	static std::string staticURI = "/static";
	std::string uri = request.getURI();

	Poco::Logger& logger = Poco::Logger::get("mq.web.access");
	logger.information(Poco::Logger::format("IP: $0 URI: $1 ($2)", request.clientAddress().toString(), uri, request.getMethod()));

	if ( ! filter(request) )
	{
		return new DenyRequestHandler();
	}

	if ( ! uri.compare(0, staticURI.size(), staticURI) 
		|| ! uri.compare("/favicon.ico") )
	{
		return new StaticRequestHandler();
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
