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
#include "MQ/Web/ChannelInitiatorMapper.h"

namespace MQ {
namespace Web {


ChannelInitiatorMapper::ChannelInitiatorMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "ChannelInitiator", input)
{
}

ChannelInitiatorMapper::~ChannelInitiatorMapper()
{
}


void ChannelInitiatorMapper::change()
{
	poco_assert_dbg(false); // Not implemented
}


void ChannelInitiatorMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not implemented
}


void ChannelInitiatorMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not implemented
}


Poco::JSON::Array::Ptr ChannelInitiatorMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_CHANNEL_INIT);

	// Optional parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");

	PCF::Vector commandResponse;
	execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
