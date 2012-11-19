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

#ifndef _MQWeb_Controller_h
#define _MQWeb_Controller_h

#include <Poco/SharedPtr.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <Poco/JSON/Object.h>

#include <MQ/QueueManager.h>
#include <MQ/CommandServer.h>

namespace MQ
{

namespace Web
{
  class Controller
  {
  public:
    Controller(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
      /// Constructor

    virtual ~Controller();
      /// Destructor


    virtual void handle() = 0;
      /// Overload this function to handle the request and send a response


	static std::string htmlize(const std::string& str);

  protected:
    QueueManager::Ptr _qmgr;


    CommandServer::Ptr _commandServer;


    Poco::Net::HTTPServerRequest& _request;


    Poco::Net::HTTPServerResponse& _response;


    Poco::JSON::Object::Ptr _data;


    void renderTemplate(const std::string& templateFile);
  };

} } // Namespace MQ::Web

#endif // _MQWeb_Controller_h
