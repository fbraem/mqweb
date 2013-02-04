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
#include <MQ/Web/ChannelMapper.h>
#include <MQ/MQException.h>

namespace MQ {
namespace Web {

typedef std::map<std::string, MQLONG> ChannelTypeFilterMap;

ChannelTypeFilterMap initializeTypeFilterMap()
{
	ChannelTypeFilterMap map;

	map["all"] = MQCHT_ALL;
	map["sender"] = MQCHT_SENDER;
	map["server"] = MQCHT_SERVER;
	map["receiver"] = MQCHT_RECEIVER;
	map["requester"] = MQCHT_REQUESTER;
	map["svrconn"] = MQCHT_SVRCONN;
	map["clntconn"] = MQCHT_CLNTCONN;
	map["clusrcvr"] = MQCHT_CLUSRCVR;
	map["clussdr"] = MQCHT_CLUSSDR;
#ifdef MQCHT_MQTT
	map["mqtt"] = MQCHT_MQTT;
#endif

	return map;
}

typedef std::map<MQLONG, std::string> ChannelTypeMap;

ChannelMapper::ChannelMapper(CommandServer& commandServer) : MQMapper(commandServer)
{
}

ChannelMapper::~ChannelMapper()
{
}


void ChannelMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void ChannelMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ChannelMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ChannelMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr jsonChannels = new Poco::JSON::Array();

	PCF::Ptr inquireChl = _commandServer.createCommand(MQCMD_INQUIRE_CHANNEL);

	inquireChl->addParameter(MQCACH_CHANNEL_NAME, filter->optValue<std::string>("name", "*"));

	std::string channelType = filter->optValue<std::string>("type", "All");
	MQLONG channelTypeValue = _dictionary.getDisplayId(MQIACH_CHANNEL_TYPE, channelType);
	poco_assert_dbg(channelTypeValue != -1);
	if ( channelTypeValue == - 1 )
	{
		return jsonChannels;
	}
	inquireChl->addParameter(MQIACH_CHANNEL_TYPE, channelTypeValue);

	PCF::Vector commandResponse;
	_commandServer.sendCommand(inquireChl, commandResponse);
	if ( commandResponse.size() > 0 )
	{
		PCF::Vector::iterator it = commandResponse.begin();
		if ( (*it)->getReasonCode() != MQCC_OK )
		{
			if ( (*it)->getReasonCode() != MQRC_UNKNOWN_OBJECT_NAME )
			{
				throw MQException(_commandServer.qmgr().name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
			}
		}

		bool excludeSystem = filter->optValue("excludeSystem", false);

		for(; it != commandResponse.end(); it++)
		{
			if ( (*it)->getReasonCode() == MQRC_UNKNOWN_OBJECT_NAME )
				break;

			if ( (*it)->isExtendedResponse() ) // Skip extended response
				continue;

			std::string channelName = (*it)->getParameterString(MQCACH_CHANNEL_NAME);
			if (    excludeSystem
				&& channelName.compare(0, 7, "SYSTEM.") == 0 )
			{
				continue;
			}

			Poco::JSON::Object::Ptr jsonChannel = new Poco::JSON::Object();
			jsonChannels->add(jsonChannel);

			mapToJSON(**it, jsonChannel);
		}
	}

	return jsonChannels;
}

}} //  Namespace MQ::Web
