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
#include "MQ/Web/ChannelMapper.h"
#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"

namespace MQ {
namespace Web {


ChannelMapper::ChannelMapper(CommandServer& commandServer) : MQMapper(commandServer, "Channel")
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

	Poco::JSON::Array::Ptr channels = new Poco::JSON::Array();

	PCF::Ptr inquireChl = _commandServer.createCommand(MQCMD_INQUIRE_CHANNEL);

	inquireChl->addParameter(MQCACH_CHANNEL_NAME, filter->optValue<std::string>("name", "*"));

	std::string channelType = filter->optValue<std::string>("type", "All");
	MQLONG channelTypeValue = dictionary()->getDisplayId(MQIACH_CHANNEL_TYPE, channelType);
	poco_assert_dbg(channelTypeValue != -1);
	if ( channelTypeValue == - 1 )
	{
		return channels;
	}
	inquireChl->addParameter(MQIACH_CHANNEL_TYPE, channelTypeValue);

	PCF::Vector commandResponse;
	_commandServer.sendCommand(inquireChl, commandResponse);

	bool excludeSystem = filter->optValue("excludeSystem", false);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		std::string channelName = (*it)->getParameterString(MQCACH_CHANNEL_NAME);
		if (    excludeSystem
			&& channelName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		Poco::JSON::Object::Ptr channel = new Poco::JSON::Object();
		channels->add(channel);

		dictionary()->mapToJSON(**it, channel);
	}

	return channels;
}

}} //  Namespace MQ::Web
