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
#include <cmqc.h>

#include <MQ/Web/MQRequestHandler.h>
#include <MQ/MQException.h>
#include <MQ/Web/QueueController.h>
#include <MQ/Web/QueueManagerController.h>
#include <MQ/Web/QueueListController.h>
#include <MQ/Web/QueueController.h>
#include <MQ/Web/MessageListController.h>
#include <MQ/Web/MessageController.h>
#include <MQ/Web/ChannelListController.h>
#include <MQ/Web/ChannelController.h>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>
#include <Poco/NullStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Logger.h>

#include <Poco/Thread.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ
{
namespace Web
{

void MQRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
  response.setChunkedTransferEncoding(true);
  response.setContentType("text/html");

  Poco::URI uri(request.getURI());
  std::vector<std::string> paths;
  uri.getPathSegments(paths);

  if ( !paths.empty()
       && paths[0].compare("favicon.ico") == 0 )
  {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    return;
  }

  try
  {
    if ( paths.size() < 2 )
    {
      QueueManagerController controller(request, response);
      controller.handle();
    }
    else if ( paths.size() > 1 )
    {
      if ( paths[1].compare("Queues") == 0 )
      {
        if ( paths.size() > 3 )
        {
          if ( paths[3].compare("Messages") == 0 )
          {
            if ( paths.size() > 4 )
            {
              MessageController controller(request, response);
              controller.handle();
            }
            else
            {
              MessageListController controller(request, response);
              controller.handle();
            }
          }
        }
        else if ( paths.size() > 2 )
        {
         QueueController controller(request, response);
         controller.handle();
        }
        else
        {
          QueueListController controller(request, response);
          controller.handle();
        }
      }
      else if ( paths[1].compare("Channels") == 0 )
      {
        if ( paths.size() > 2 )
        {
          ChannelController controller(request, response);
          controller.handle();
        }
        else
        {
          ChannelListController controller(request, response);
          controller.handle();
        }
      }
    }
  }
  catch(MQException mqe)
  {
    Poco::JSON::Object::Ptr error = new Poco::JSON::Object();
    error->set("object", mqe.object());
    error->set("function", mqe.function());
    switch(mqe.code())
    {
      case MQCC_OK: error->set("code", "OK"); break;
      case MQCC_WARNING: error->set("code", "WARNING"); break;
      case MQCC_FAILED: error->set("code", "ERROR"); break;
    }
    error->set("reason", mqe.reason());

    Poco::Net::HTMLForm form(request);
    std::string format = form.get("format", "");
    if ( format.compare("json") == 0 )
    {
      error->stringify(response.send());
      response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    }
    else
    {
      Poco::JSON::Object::Ptr data = new Poco::JSON::Object();
      data->set("error", error);

      Poco::JSON::Template::Ptr tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path("error.tpl"));
      poco_assert_dbg(!tpl.isNull());

      tpl->render(data, response.send());
      response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    }
  }
  catch(Poco::FileNotFoundException fnfe)
  {
    std::cout << "Can't find file " << fnfe.message() << std::endl;
    response.setStatus(Poco::Net::HTTPResponse::HTTP_REASON_BAD_REQUEST);
  }
  // Make sure everything is read, otherwise this can result
  // in Bad Request error in the next call.
  Poco::NullOutputStream nos;
  Poco::StreamCopier::copyStream(request.stream(), nos);
}

} } // Namespace MQ::Web
