/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or ? as soon they
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
#include "MQ/Web/ListenerMapper.h"

namespace MQ {
namespace Web {

ListenerMapper::ListenerMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "Listener", input)
{
}

ListenerMapper::~ListenerMapper()
{
}


void ListenerMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void ListenerMapper::create(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void ListenerMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ListenerMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_LISTENER);

	// Required parameters
	addParameter<std::string>(MQCACH_LISTENER_NAME, "ListenerName");

	// Optional parameters
	addIntegerFilter();
	addAttributeList(MQIACF_LISTENER_ATTRS, "ListenerAttrs");
	addStringFilter();
	addParameterNumFromString(MQIACH_XMIT_PROTOCOL_TYPE, "TransportType");

	PCF::Vector commandResponse;
	execute(commandResponse);

	bool excludeSystem = _input->optValue("ExcludeSystem", false);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE )
			continue;

		std::string listenerName = (*it)->getParameterString(MQCACH_LISTENER_NAME);
		if ( excludeSystem
			&& listenerName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
