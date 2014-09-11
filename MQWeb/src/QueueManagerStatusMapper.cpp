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

namespace MQ {
namespace Web {


QueueManagerStatusMapper::QueueManagerStatusMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "QueueManagerStatus", input)
{
}

QueueManagerStatusMapper::~QueueManagerStatusMapper()
{
}


void QueueManagerStatusMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueManagerStatusMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void QueueManagerStatusMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueManagerStatusMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_Q_MGR_STATUS);

	// Optional parameters
	addAttributeList(MQIACF_Q_MGR_STATUS_ATTRS, "QMStatusAttrs");

	std::vector<Poco::SharedPtr<PCF> > commandResponse;
	execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
