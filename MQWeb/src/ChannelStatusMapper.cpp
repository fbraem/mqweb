/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
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

namespace MQ {
namespace Web {


ChannelStatusMapper::ChannelStatusMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "ChannelStatus", input)
{
}

ChannelStatusMapper::~ChannelStatusMapper()
{
}


void ChannelStatusMapper::change()
{
  poco_assert_dbg(false); // Not applicable
}


void ChannelStatusMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not applicable
}


void ChannelStatusMapper::copy(bool replace)
{
  poco_assert_dbg(false); // Not applicable
}


Poco::JSON::Array::Ptr ChannelStatusMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_CHANNEL_STATUS);

	// Required parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");

	/*TODO: move to controller!
	addParameterNumFromString(MQIACH_CHANNEL_TYPE, "ChannelType");
	if ( filter->has("type") )
	{
		std::string channelType = filter->optValue<std::string>("type", "All");
		MQLONG channelTypeValue = dictionary()->getDisplayId(MQIACH_CHANNEL_TYPE, channelType);
		poco_assert_dbg(channelTypeValue != -1);
		if ( channelTypeValue != MQCHT_ALL )
		{
			inquireChlStatus->addFilter(MQIACH_CHANNEL_TYPE, MQCFOP_EQUAL, channelTypeValue);
		}
	}*/

	// Optional parameters
	addParameterNumFromString(MQIACH_CHANNEL_DISP, "ChannelDisposition");
	addParameter<std::string>(MQCACH_CLIENT_ID, "ClientIdentifier");
	addAttributeList(MQIACH_CHANNEL_INSTANCE_ATTRS, "ChannelInstanceAttrs");
	addParameterNumFromString(MQIACH_CHANNEL_INSTANCE_TYPE, "ChannelInstanceType");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addParameter<std::string>(MQCACH_CONNECTION_NAME, "ConnectionName");
	addIntegerFilter();
	addStringFilter();
	addParameter<std::string>(MQCACH_XMIT_Q_NAME, "XmitQName");

	PCF::Vector commandResponse;
	execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip Extended Response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
