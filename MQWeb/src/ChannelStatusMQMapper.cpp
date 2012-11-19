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
#include <MQ/Web/ChannelStatusMQMapper.h>
#include <MQ/Web/ChannelMQMapper.h>
#include <MQ/MQException.h>

namespace MQ {
namespace Web {

ChannelStatusMQMapper::ChannelStatusMQMapper(CommandServer& commandServer) : MQMapper(commandServer)
{
}

ChannelStatusMQMapper::~ChannelStatusMQMapper()
{
}


void ChannelStatusMQMapper::change(const Poco::JSON::Object::Ptr&obj)
{
  poco_assert_dbg(false); // Not applicable
}


void ChannelStatusMQMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not applicable
}


void ChannelStatusMQMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not applicable
}


Poco::JSON::Array::Ptr ChannelStatusMQMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
  poco_assert_dbg(!filter.isNull());

  Poco::JSON::Array::Ptr jsonStatuses = new Poco::JSON::Array();

  PCF::Ptr inquireChlStatus = _commandServer.createCommand(MQCMD_INQUIRE_CHANNEL_STATUS);
  inquireChlStatus->addParameter(MQCACH_CHANNEL_NAME, filter->optValue<std::string>("name", "*"));
  //TODO: use a filter for inquiring instance type
  inquireChlStatus->addParameter(MQIACH_CHANNEL_INSTANCE_TYPE, MQOT_CURRENT_CHANNEL);

  PCF::Vector commandResponse;
  _commandServer.sendCommand(inquireChlStatus, commandResponse);
  if ( commandResponse.size() > 0 )
  {
    PCF::Vector::iterator it = commandResponse.begin();
    if ( (*it)->getCompletionCode() != MQCC_OK )
    {
	  if ( (*it)->getReasonCode() == MQRCCF_NONE_FOUND ) // Nothing found
	  {
		  return jsonStatuses;
	  }

      if ( (*it)->getReasonCode() != MQRCCF_CHL_STATUS_NOT_FOUND )
      {
        throw MQException(_commandServer.qmgr().name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
      }
    }

    for(; it != commandResponse.end(); it++)
    {
	  if ( (*it)->isExtendedResponse() ) // Skip Extended Response
		  continue;

      if ( (*it)->getReasonCode() == MQRCCF_CHL_STATUS_NOT_FOUND )
        break;

      Poco::JSON::Object::Ptr jsonChannelStatus = new Poco::JSON::Object();
      jsonStatuses->add(jsonChannelStatus);

      jsonChannelStatus->set("channelName", (*it)->getParameterString(MQCACH_CHANNEL_NAME));

      Poco::JSON::Object::Ptr jsonType = new Poco::JSON::Object();
      jsonChannelStatus->set("channelType", jsonType);
      std::string type = ChannelMQMapper::getType((*it)->getParameterNum(MQIACH_CHANNEL_TYPE));
      jsonType->set("value", type);
      jsonType->set(type, true);

      Poco::JSON::Object::Ptr jsonStatus = new Poco::JSON::Object();
      jsonChannelStatus->set("channelStatus", jsonStatus);
      switch((*it)->getParameterNum(MQIACH_CHANNEL_STATUS))
      {
      case MQCHS_BINDING:
        jsonStatus->set("value", "binding");
        break;
      case MQCHS_STARTING:
        jsonStatus->set("value", "starting");
        break;
      case MQCHS_RUNNING:
        jsonStatus->set("value", "running");
        break;
      case MQCHS_PAUSED:
        jsonStatus->set("value", "paused");
        break;
      case MQCHS_STOPPING:
        jsonStatus->set("value", "stopping");
        break;
      case MQCHS_RETRYING:
        jsonStatus->set("value", "retrying");
        break;
      case MQCHS_STOPPED:
        jsonStatus->set("value", "stopped");
        break;
      case MQCHS_REQUESTING:
        jsonStatus->set("value", "requesting");
        break;
      case MQCHS_INITIALIZING:
        jsonStatus->set("value", "initializing");
        break;
      }
      jsonStatus->set(jsonStatus->getValue<std::string>("value"), true);
      mapDateToJSON(*it, jsonChannelStatus, "startDate", MQCACH_CHANNEL_START_DATE, MQCACH_CHANNEL_START_DATE);
    }
  }

  return jsonStatuses;
}

}} //  Namespace MQ::Web
