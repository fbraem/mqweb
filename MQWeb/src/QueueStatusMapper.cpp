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
#include "MQ/Web/QueueStatusMapper.h"
#include "MQ/Web/Dictionary.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {

QueueStatusMapper::QueueStatusMapper(CommandServer& commandServer) : MQMapper(commandServer, "QueueStatus")
{
}

QueueStatusMapper::~QueueStatusMapper()
{
}


void QueueStatusMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueStatusMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueStatusMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueStatusMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

	PCF::Ptr command = _commandServer.createCommand(MQCMD_INQUIRE_Q_STATUS);
	command->addParameter(MQCA_Q_NAME, filter->optValue<std::string>("queueName", "*"));

	if ( filter->has("openType") )
	{
		std::string type = filter->optValue<std::string>("openType", "All");
		MQLONG typeValue = dictionary()->getDisplayId(MQIACF_OPEN_TYPE, type);
		poco_assert_dbg(typeValue != -1);
		if ( typeValue != MQQSOT_ALL )
		{
			command->addParameter(MQIACF_OPEN_TYPE, typeValue);
		}
	}

	if ( filter->has("statusType") )
	{
		std::string type = filter->optValue<std::string>("statusType", "Queue Status");
		MQLONG typeValue = dictionary()->getDisplayId(MQIACF_STATUS_TYPE, type);
		poco_assert_dbg(typeValue != -1);
		command->addParameter(MQIACF_STATUS_TYPE, typeValue);
	}

	PCF::Vector commandResponse;
	_commandServer.sendCommand(command, commandResponse);

	bool excludeSystem = filter->optValue("excludeSystem", false);
	bool excludeTemp = filter->optValue("excludeTemp", false);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		std::string qName = (*it)->getParameterString(MQCA_Q_NAME);
		if ( excludeSystem
			&& qName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		if ( excludeTemp
			&& (*it)->hasParameter(MQIA_DEFINITION_TYPE)
			&& (*it)->getParameterNum(MQIA_DEFINITION_TYPE) == MQQDT_TEMPORARY_DYNAMIC )
		{
			continue;
		}

		Poco::JSON::Object::Ptr jsonStatus = new Poco::JSON::Object();
		json->add(jsonStatus);

		dictionary()->mapToJSON(**it, jsonStatus);
	}

	return json;
}

}} //  Namespace MQ::Web
