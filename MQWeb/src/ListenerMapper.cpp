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
#include "MQ/Web/Dictionary.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {

ListenerMapper::ListenerMapper(CommandServer& commandServer) : MQMapper(commandServer, "Listener")
{
}

ListenerMapper::~ListenerMapper()
{
}


void ListenerMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void ListenerMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void ListenerMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ListenerMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Command command(this, MQCMD_INQUIRE_LISTENER, filter);

	// Required parameters
	command.addParameter<std::string>(MQCACH_LISTENER_NAME, "ListenerName");

	// Optional parameters
	command.addIntegerFilter();
	command.addAttributeList(MQIACF_LISTENER_ATTRS, "ListenerAttrs");
	command.addStringFilter();
	command.addParameterNumFromString(MQIACH_XMIT_PROTOCOL_TYPE, "TransportType");

	PCF::Vector commandResponse;
	command.execute(commandResponse);

	bool excludeSystem = filter->optValue("ExcludeSystem", false);

	Poco::JSON::Array::Ptr listeners = new Poco::JSON::Array();
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

		Poco::JSON::Object::Ptr listener = new Poco::JSON::Object();
		listeners->add(listener);

		dictionary()->mapToJSON(**it, listener);
	}

	return listeners;
}

}} //  Namespace MQ::Web
