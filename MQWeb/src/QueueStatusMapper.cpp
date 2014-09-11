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

namespace MQ {
namespace Web {

QueueStatusMapper::QueueStatusMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "QueueStatus", input)
{
}

QueueStatusMapper::~QueueStatusMapper()
{
}


void QueueStatusMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueStatusMapper::create(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueStatusMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueStatusMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_Q_STATUS);

	// Required parameters
	addParameter<std::string>(MQCA_Q_NAME, "QName");

	// Optional parameters
	//TODO: ByteStringFilter
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addParameterNumFromString(MQIACF_OPEN_TYPE, "OpenType");
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	addAttributeList(MQIACF_Q_STATUS_ATTRS, "QStatusAttrs");
	addParameterNumFromString(MQIACF_STATUS_TYPE, "StatusType");
	addStringFilter();

	PCF::Vector commandResponse;
	execute(commandResponse);

	bool excludeSystem = _input->optValue("ExcludeSystem", false);
	bool excludeTemp = _input->optValue("ExcludeTemp", false);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

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

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
