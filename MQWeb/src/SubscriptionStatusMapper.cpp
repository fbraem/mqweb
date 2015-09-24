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
#include "MQ/Web/SubscriptionStatusMapper.h"

#include "Poco/HexBinaryDecoder.h"

namespace MQ {
namespace Web {


SubscriptionStatusMapper::SubscriptionStatusMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "SubStatus", input)
{
}

SubscriptionStatusMapper::~SubscriptionStatusMapper()
{
}


void SubscriptionStatusMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void SubscriptionStatusMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void SubscriptionStatusMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr SubscriptionStatusMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_SUB_STATUS);

	// Required parameters
	addParameter<std::string>(MQCACF_SUB_NAME, "SubName");

	if ( _input->has("SubId") )
	{
		std::string hexId = _input->get("SubId");
		if ( hexId.length() > MQ_CORREL_ID_LENGTH )
		{
			hexId.erase(MQ_CORREL_ID_LENGTH);
		}
		Buffer::Ptr id = new Buffer(hexId);

		pcf()->addParameter(MQBACF_SUB_ID, id);
	}

	// Optional parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addParameter<MQLONG>(MQIACF_DURABLE_SUBSCRIPTION, "Durable");
	addParameterNumFromString(MQIACF_SUB_TYPE, "SubscriptionType");
	addAttributeList(MQIACF_SUB_STATUS_ATTRS, "StatusAttrs");

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
