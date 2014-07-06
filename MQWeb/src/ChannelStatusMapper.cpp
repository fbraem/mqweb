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
#include "MQ/Web/ChannelStatusMapper.h"
#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"

namespace MQ {
namespace Web {


ChannelStatusMapper::ChannelStatusMapper(CommandServer& commandServer) : MQMapper(commandServer, "ChannelStatus")
{
}

ChannelStatusMapper::~ChannelStatusMapper()
{
}


void ChannelStatusMapper::change(const Poco::JSON::Object::Ptr&obj)
{
  poco_assert_dbg(false); // Not applicable
}


void ChannelStatusMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not applicable
}


void ChannelStatusMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not applicable
}


Poco::JSON::Array::Ptr ChannelStatusMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr jsonStatuses = new Poco::JSON::Array();

	PCF::Ptr inquireChlStatus = _commandServer.createCommand(MQCMD_INQUIRE_CHANNEL_STATUS);

	inquireChlStatus->addParameter(MQCACH_CHANNEL_NAME, filter->optValue<std::string>("name", "*"));

	if ( filter->has("type") )
	{
		std::string channelType = filter->optValue<std::string>("type", "All");
		MQLONG channelTypeValue = dictionary()->getDisplayId(MQIACH_CHANNEL_TYPE, channelType);
		poco_assert_dbg(channelTypeValue != -1);
		if ( channelTypeValue != MQCHT_ALL )
		{
			inquireChlStatus->addFilter(MQIACH_CHANNEL_TYPE, MQCFOP_EQUAL, channelTypeValue);
		}
	}

	MQLONG instanceTypeValue = MQOT_CURRENT_CHANNEL;
	if ( filter->has("instanceType") )
	{
		std::string instanceType = filter->optValue<std::string>("type", "Current");
		instanceTypeValue = dictionary()->getDisplayId(MQIACH_CHANNEL_INSTANCE_TYPE, instanceType);
		poco_assert_dbg(instanceTypeValue != -1);
	}
	inquireChlStatus->addParameter(MQIACH_CHANNEL_INSTANCE_TYPE, instanceTypeValue);

	PCF::Vector commandResponse;
	_commandServer.sendCommand(inquireChlStatus, commandResponse);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip Extended Response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		Poco::JSON::Object::Ptr jsonChannelStatus = new Poco::JSON::Object();
		dictionary()->mapToJSON(**it, jsonChannelStatus);
		jsonStatuses->add(jsonChannelStatus);
	}

	return jsonStatuses;
}

}} //  Namespace MQ::Web
