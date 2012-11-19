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
#include <Poco/DateTimeFormatter.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>

#include <MQ/MQException.h>
#include <MQ/CommandServer.h>

#include <MQ/Web/ChannelController.h>
#include <MQ/Web/ChannelMQMapper.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ
{
namespace Web
{

ChannelController::ChannelController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : Controller(request, response)
{
}


ChannelController::~ChannelController()
{

}


void ChannelController::handle()
{
  Poco::URI uri(_request.getURI());
  std::vector<std::string> paths;
  uri.getPathSegments(paths);

  if ( paths.size() < 3 )
  {
    _response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    return;
  }

  std::string channelName = paths[2];

  Poco::JSON::Object::Ptr jsonQmgr = _data->getObject("qmgr");
  poco_assert_dbg(!jsonQmgr.isNull());

  jsonQmgr->set("createDate", Poco::DateTimeFormatter::format(_qmgr->creationDate(), "%d-%m-%Y %H:%M:%S"));
  jsonQmgr->set("alterDate", Poco::DateTimeFormatter::format(_qmgr->alterationDate(), "%d-%m-%Y %H:%M:%S"));
  jsonQmgr->set("id", _qmgr->id());
  jsonQmgr->set("description", _qmgr->description());

  Poco::Net::HTMLForm form(_request);
  std::string type = form.get("type", "all");

  Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();
  filter->set("name", channelName);
  filter->set("type", type);

  ChannelMQMapper channelMapper(*_commandServer.get());
  Poco::JSON::Array::Ptr jsonChannels = channelMapper.inquire(filter);
  Poco::JSON::Object::Ptr jsonChannel = jsonChannels->getObject(0);

  if ( !jsonChannel.isNull() )
  {
    _data->set("channel", jsonChannel);
  }

  renderTemplate("channel.tpl");
}


} } // Namespace MQ::Web
