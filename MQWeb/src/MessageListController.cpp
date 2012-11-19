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
#include <iomanip>

#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>
#include <Poco/DateTimeFormatter.h>

#include <Poco/JSON/TemplateCache.h>

#include <MQ/PCF.h>
#include <MQ/MQException.h>
#include <MQ/Web/MessageListController.h>
#include <MQ/QueueManager.h>
#include <MQ/Queue.h>
#include <MQ/CommandServer.h>

namespace MQ
{
namespace Web
{

MessageListController::MessageListController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : Controller(request, response)
{
}


MessageListController::~MessageListController()
{

}


void MessageListController::handle()
{
  if ( _request.getMethod().compare("POST") != 0 )
  {
    _response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    return;
  }

  Poco::URI uri(_request.getURI());
  std::vector<std::string> paths;
  uri.getPathSegments(paths);

  std::string queueName = paths[2];
  Poco::JSON::Object::Ptr jsonQueue = new Poco::JSON::Object();
  jsonQueue->set("name", queueName);
  _data->set("queue", jsonQueue);

  Poco::Net::HTMLForm form(_request, _request.stream());

  std::string limitField = form.get("limit");
  int limit = -1;
  if ( ! limitField.empty() )
  {
    Poco::NumberParser::tryParse(limitField, limit);
  }

  std::string teaserField = form.get("teaser");
  int teaser = 0;
  if ( ! teaserField.empty() )
  {
    Poco::NumberParser::tryParse(teaserField, teaser);
  }

  Poco::JSON::Array::Ptr jsonMessages = new Poco::JSON::Array();

  Queue q(_qmgr, queueName);
  q.open(MQOO_BROWSE);

  int count = 0;
  while(1)
  {
    Message msg(teaser);
    try
    {
      q.get(msg, MQGMO_BROWSE_NEXT + MQGMO_ACCEPT_TRUNCATED_MSG, 0);
    }
    catch(MQException mqe)
    {
      if ( mqe.reason() == MQRC_NO_MSG_AVAILABLE )
      {
        break;
      }
      if ( mqe.reason() != MQRC_TRUNCATED_MSG_ACCEPTED )
      {
        throw;
      }
    }

    count++;
    Poco::JSON::Object::Ptr jsonMessage = new Poco::JSON::Object();

    BufferPtr id = msg.getMessageId();
    std::stringstream hexId;
    for(int i = 0; i < id->size(); ++i)
    {
      hexId << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int) (*id)[i];
    }
    jsonMessage->set("id", hexId.str());

    jsonMessage->set("putDate", Poco::DateTimeFormatter::format(msg.getPutDate(), "%d-%m-%Y %H:%M:%S"));
    jsonMessage->set("user", msg.getUser());
    jsonMessage->set("putApplication", msg.getPutApplication());
    jsonMessage->set("format", msg.getFormat());
    jsonMessage->set("length", msg.dataLength());
	jsonMessage->set("encoding", msg.getEncoding());
	jsonMessage->set("ccsid", msg.getCodedCharSetId());

    std::string data;
    if (    teaser > 0
         && msg.getFormat().compare(MQFMT_STRING) == 0 )
    {
      if ( msg.dataLength() < msg.buffer().size() )
      {
        msg.buffer().resize(msg.dataLength());
      }
      data = std::string(msg.buffer().begin(), msg.buffer().end());
      if ( teaser < msg.dataLength() )
      {
        data += " ...";
      }
      jsonMessage->set("data", data);
    }

    jsonMessages->add(jsonMessage);
  }

  _data->set("messages", jsonMessages);

  Poco::JSON::Template::Ptr tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path("messageList.tpl"));
  poco_assert_dbg(!tpl.isNull());

  tpl->render(_data, _response.send());
  _response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
}

} } // Namespace MQ::Web
