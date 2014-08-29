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
#include "MQ/Web/QueueManagerStatusMapper.h"
#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"

namespace MQ {
namespace Web {


QueueManagerStatusMapper::QueueManagerStatusMapper(CommandServer& commandServer) : MQMapper(commandServer, "QueueManagerStatus")
{
}

QueueManagerStatusMapper::~QueueManagerStatusMapper()
{
}


void QueueManagerStatusMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueManagerStatusMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void QueueManagerStatusMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueManagerStatusMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	Command command(this, MQCMD_INQUIRE_Q_MGR_STATUS, filter);

	// Optional parameters
	command.addAttributeList(MQIACF_Q_MGR_STATUS_ATTRS, "QMStatusAttrs");

	std::vector<Poco::SharedPtr<PCF> > commandResponse;
	command.execute(commandResponse);

	Poco::JSON::Array::Ptr jsonQueueManagerStatuses = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		Poco::JSON::Object::Ptr jsonQmgr = new Poco::JSON::Object();
		jsonQueueManagerStatuses->add(jsonQmgr);

		dictionary()->mapToJSON(**it, jsonQmgr);
	}

	return jsonQueueManagerStatuses;
}

}} //  Namespace MQ::Web
