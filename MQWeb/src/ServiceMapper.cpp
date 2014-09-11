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
#include "MQ/Web/ServiceMapper.h"

namespace MQ {
namespace Web {


ServiceMapper::ServiceMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "Service", input)
{
}

ServiceMapper::~ServiceMapper()
{
}


void ServiceMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void ServiceMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ServiceMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ServiceMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_SERVICE);

	// Required parameters
	addParameter<std::string>(MQCA_SERVICE_NAME, "ServiceName");

	// Optional parameters
	addIntegerFilter();
	addAttributeList(MQIACF_SERVICE_ATTRS, "ServiceAttrs");
	addStringFilter();

	PCF::Vector commandResponse;
	execute(commandResponse);

	bool excludeSystem = _input->optValue("ExcludeSystem", false);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		std::string serviceName = (*it)->getParameterString(MQCA_SERVICE_NAME);
		if ( excludeSystem && serviceName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
