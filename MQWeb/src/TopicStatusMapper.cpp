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
#include "MQ/Web/TopicStatusMapper.h"

namespace MQ {
namespace Web {


TopicStatusMapper::TopicStatusMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "Topic", input)
{
}

TopicStatusMapper::~TopicStatusMapper()
{
}


void TopicStatusMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void TopicStatusMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void TopicStatusMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr TopicStatusMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_TOPIC_STATUS);

	// Required parameters
	addParameter<std::string>(MQCA_TOPIC_STRING, "TopicString");

	// Optional parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addStringFilter();
	addAttributeList(MQIACF_TOPIC_ATTRS, "TopicStatusAttrs");
	addParameterNumFromString(MQIACF_TOPIC_STATUS_TYPE, "StatusType");

	PCF::Vector commandResponse;
	execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
