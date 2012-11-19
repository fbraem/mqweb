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
#include <Poco/Net/HTMLForm.h>

#include <MQ/MQException.h>
#include <MQ/CommandServer.h>

#include <MQ/Web/ChannelListController.h>
#include <MQ/Web/ChannelMQMapper.h>
#include <MQ/Web/ChannelStatusMQMapper.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ {
namespace Web {


ChannelListController::ChannelListController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : Controller(request, response)
{
  poco_assert_dbg(!_commandServer.isNull()); // Can't be null
}


ChannelListController::~ChannelListController()
{

}

void ChannelListController::handle()
{
  if ( _request.getMethod().compare("POST") != 0 )
  {
    _response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    return;
  }

  Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

  Poco::Net::HTMLForm form(_request, _request.stream());

  std::string channelNameField = form.get("channelName", "*");
  filter->set("name", channelNameField.empty() ? "*" : channelNameField);
  filter->set("type", form.get("channelType", "all"));
  filter->set("excludeSystem", form.get("channelExcludeSystem", "0").compare("1") == 0);

  ChannelMQMapper channelMapper(*_commandServer.get());
  Poco::JSON::Array::Ptr jsonChannels = channelMapper.inquire(filter);

  // A channel name is not unique. To make it possible to associate the status
  // to the correct channel, we store all channels in a temporary JSON object
  Poco::JSON::Object::Ptr jsonAllChannels = new Poco::JSON::Object();
  for(Poco::JSON::Array::ValueVector::const_iterator it = jsonChannels->begin(); it != jsonChannels->end(); ++it)
  {
    if ( it->type() == typeid(Poco::JSON::Object::Ptr) )
    {
      Poco::JSON::Object::Ptr jsonChannel = it->extract<Poco::JSON::Object::Ptr>();

      std::string channelName = jsonChannel->getValue<std::string>("channelName");

      Poco::JSON::Object::Ptr jsonType = jsonChannel->getObject("channelType");
      poco_assert_dbg(! jsonType.isNull());
      std::string type = jsonType->get("value");

      jsonAllChannels->set(type + "/" + channelName, *it);
    }
  }

  filter = new Poco::JSON::Object();
  filter->set("name", channelNameField.empty() ? "*" : channelNameField);
  ChannelStatusMQMapper channelStatusMapper(*_commandServer.get());
  Poco::JSON::Array::Ptr statuses = channelStatusMapper.inquire(filter);

  // Associate all status objects to their corresponding channel object
  for(Poco::JSON::Array::ValueVector::const_iterator it = statuses->begin(); it != statuses->end(); ++it)
  {
    if ( it->type() == typeid(Poco::JSON::Object::Ptr) )
    {
      Poco::JSON::Object::Ptr jsonStatus = it->extract<Poco::JSON::Object::Ptr>();

      std::string channelName = jsonStatus->getValue<std::string>("channelName");

      Poco::JSON::Object::Ptr jsonType = jsonStatus->getObject("channelType");
      poco_assert_dbg(! jsonType.isNull());
      std::string type = jsonType->get("value");

      Poco::JSON::Object::Ptr jsonChannel = jsonAllChannels->getObject(type + "/" + channelName);
      if ( jsonChannel.isNull() )
      {
        jsonChannel = new Poco::JSON::Object();
        jsonChannel->set("channelName", channelName);
        jsonChannel->set("channelType", jsonType);
        jsonChannel->set("autodefined", true);
        jsonAllChannels->set(type + "/" + channelName, jsonChannel);
        jsonChannels->add(jsonChannel);
      }
      jsonChannel->set("status", jsonStatus);
    }
  }

  _data->set("channels", jsonChannels);

  renderTemplate("channelList.tpl");
}

} } // Namespace MQ::Web
