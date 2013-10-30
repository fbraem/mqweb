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

#include <MQ/Web/DenyRequestHandler.h>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeParser.h>
#include <Poco/Util/Application.h>
#include <Poco/URI.h>
#include <Poco/Path.h>
#include <Poco/File.h>

namespace MQ
{
namespace Web
{

DenyRequestHandler::DenyRequestHandler() 
{
}

void DenyRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
  Poco::Logger& logger = Poco::Logger::get("mq.web.access");

  std::string ip = request.clientAddress().host().toString();
  logger.warning(Poco::Logger::format("$0 : HTTP_FORBIDDEN", ip));

  response.setStatus(Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
  response.send();
}

} } // Namespace MQ::Web
