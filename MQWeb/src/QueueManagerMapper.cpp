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
#include "MQ/Web/QueueManagerMapper.h"

namespace MQ {
namespace Web {


QueueManagerMapper::QueueManagerMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "QueueManager", input)
{
}

QueueManagerMapper::~QueueManagerMapper()
{
}


void QueueManagerMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueManagerMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void QueueManagerMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueManagerMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_Q_MGR);

	// Optional parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addAttributeList(MQIACF_Q_MGR_ATTRS, "QMgrAttrs");

	std::vector<Poco::SharedPtr<PCF> > commandResponse;
	execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		Poco::JSON::Object::Ptr data = new Poco::JSON::Object();
		json->add(data);
		data->set("type", "queuemanager");
		data->set("id", (*it)->getParameterString(MQCA_Q_MGR_NAME));
		data->set("attributes", createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
