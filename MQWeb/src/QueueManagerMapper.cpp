/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
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
#include "MQ/Web/QueueManagerMapper.h"
#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"

namespace MQ {
namespace Web {


QueueManagerMapper::QueueManagerMapper(CommandServer& commandServer) : MQMapper(commandServer)
{
}

QueueManagerMapper::~QueueManagerMapper()
{
}


void QueueManagerMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueManagerMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void QueueManagerMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueManagerMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	Poco::JSON::Array::Ptr jsonQueueManagers = new Poco::JSON::Array();

	PCF::Ptr inquireQmgr = _commandServer.createCommand(MQCMD_INQUIRE_Q_MGR);

	std::vector<Poco::SharedPtr<PCF> > commandResponse;
	_commandServer.sendCommand(inquireQmgr, commandResponse);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		Poco::JSON::Object::Ptr jsonQmgr = new Poco::JSON::Object();
		jsonQueueManagers->add(jsonQmgr);

		mapToJSON(**it, jsonQmgr, "QueueManager");
	}

	return jsonQueueManagers;
}

}} //  Namespace MQ::Web
