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
#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"
#include "Poco/HexBinaryDecoder.h"

namespace MQ {
namespace Web {


ConnectionMapper::ConnectionMapper(CommandServer& commandServer) : MQMapper(commandServer, "Connection")
{
}

ConnectionMapper::~ConnectionMapper()
{
}


void ConnectionMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not implemented
}


void ConnectionMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not implemented
}


void ConnectionMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not implemented
}


Poco::JSON::Array::Ptr ConnectionMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr connections = new Poco::JSON::Array();

	PCF::Ptr inquireConnections = _commandServer.createCommand(MQCMD_INQUIRE_CONNECTION);

	if ( filter->has("id") )
	{
		std::string hexId = filter->get("id");
		BufferPtr id = new Buffer(hexId.size() / 2);

		std::istringstream iss(hexId);
		Poco::HexBinaryDecoder decoder(iss);
		int c = decoder.get();
		int i = 0;
		while (c != -1 && i < id->size())
		{
			id[i++] = (unsigned char) c;
			c = decoder.get();
		}

		inquireConnections->addParameter(MQBACF_CONNECTION_ID, id);
	}
	else
	{
		BufferPtr id = new Buffer(0); // Empty buffer
		inquireConnections->addParameter(MQBACF_GENERIC_CONNECTION_ID, id);
	}

	PCF::Vector commandResponse;
	_commandServer.sendCommand(inquireConnections, commandResponse);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		Poco::JSON::Object::Ptr connection = new Poco::JSON::Object();
		connections->add(connection);

		dictionary()->mapToJSON(**it, connection);
	}

	return connections;
}

}} //  Namespace MQ::Web
