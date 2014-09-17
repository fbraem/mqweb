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
#include "MQ/Web/ChannelMapper.h"

namespace MQ {
namespace Web {


ChannelMapper::ChannelMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "Channel", input)
{
}

ChannelMapper::~ChannelMapper()
{
}


void ChannelMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void ChannelMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ChannelMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ChannelMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_CHANNEL);

	// Required Parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");

	// Optional Parameters
	addParameterNumFromString(MQIACH_CHANNEL_TYPE, "ChannelType");
	addAttributeList(MQIACF_CHANNEL_ATTRS, "ChannelAttrs");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addParameterNumFromString(MQIACH_CHANNEL_DISP, "DefaultChannelDisposition");
	addIntegerFilter();
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	addStringFilter();

	PCF::Vector commandResponse;
	execute(commandResponse);

	bool excludeSystem = _input->optValue("ExcludeSystem", false);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

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

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
