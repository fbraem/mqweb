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
#include "MQ/Web/ConnectionMapper.h"

#include "Poco/HexBinaryDecoder.h"

namespace MQ {
namespace Web {


ConnectionMapper::ConnectionMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "Connection", input)
{
}

ConnectionMapper::~ConnectionMapper()
{
}


void ConnectionMapper::change()
{
	poco_assert_dbg(false); // Not implemented
}


void ConnectionMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not implemented
}


void ConnectionMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not implemented
}


Poco::JSON::Array::Ptr ConnectionMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_CONNECTION);

	// Required parameters
	if ( _input->has("ConnectionId") )
	{
		std::string hexId = _input->get("ConnectionId");
		if ( hexId.length() > MQ_CONNECTION_ID_LENGTH )
		{
			hexId.erase(MQ_CONNECTION_ID_LENGTH);
		}
		Buffer::Ptr id = new Buffer(hexId);

		pcf()->addParameter(MQBACF_CONNECTION_ID, id);
	}
	else
	{
		Buffer::Ptr id = new Buffer(MQ_CONNECTION_ID_LENGTH); // Empty buffer
		memset(id->data(), 0, MQ_CONNECTION_ID_LENGTH);
		pcf()->addParameter(MQBACF_GENERIC_CONNECTION_ID, id);
	}

	// Optional parameters
	//TODO: ByteStringFilter
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addAttributeList(MQIACF_CONNECTION_ATTRS, "ConnectionAttrs");
	addParameterNumFromString(MQIACF_CONN_INFO_TYPE, "ConnInfoType");
	addIntegerFilter();
	addStringFilter();
	addParameterNumFromString(MQIA_UR_DISP, "URDisposition");

	if ( ! _input->has("ConnectionAttrs") )
	{
		// It seems that this is not set by default, so we do
		// it ourselves.
		MQLONG attrs[] = { MQIACF_ALL };
		pcf()->addParameterList(MQIACF_CONNECTION_ATTRS, attrs, 1);
	}

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
