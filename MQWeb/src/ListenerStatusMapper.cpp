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
#include "MQ/Web/ListenerStatusMapper.h"
#include "MQ/Web/Dictionary.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {

ListenerStatusMapper::ListenerStatusMapper(CommandServer& commandServer) : MQMapper(commandServer, "ListenerStatus")
{
}

ListenerStatusMapper::~ListenerStatusMapper()
{
}


void ListenerStatusMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void ListenerStatusMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void ListenerStatusMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ListenerStatusMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr statuses = new Poco::JSON::Array();

	PCF::Ptr inquireListenerStatus = _commandServer.createCommand(MQCMD_INQUIRE_LISTENER_STATUS);
	inquireListenerStatus->addParameter(MQCACH_LISTENER_NAME, filter->optValue<std::string>("name", "*"));

	PCF::Vector commandResponse;
	_commandServer.sendCommand(inquireListenerStatus, commandResponse);
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE )
			continue;

		Poco::JSON::Object::Ptr status = new Poco::JSON::Object();
		statuses->add(status);

		dictionary()->mapToJSON(**it, status);
	}

	return statuses;
}

}} //  Namespace MQ::Web
