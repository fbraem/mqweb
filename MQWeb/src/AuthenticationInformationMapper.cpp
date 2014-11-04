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
#include "MQ/Web/AuthenticationInformationMapper.h"

namespace MQ {
namespace Web {

AuthenticationInformationMapper::AuthenticationInformationMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "AuthenticationInformation", input)
{
}

AuthenticationInformationMapper::~AuthenticationInformationMapper()
{
}


void AuthenticationInformationMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthenticationInformationMapper::create(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthenticationInformationMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr AuthenticationInformationMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_AUTH_INFO);

	// Required parameters
	addParameter<std::string>(MQCA_AUTH_INFO_NAME, "AuthInfoName");

	// Optional parameters
	addAttributeList(MQIACF_AUTH_INFO_ATTRS, "AuthInfoAttrs");
	addParameterNumFromString(MQIA_AUTH_INFO_TYPE, "AuthInfoType");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	addStringFilter();

	PCF::Vector commandResponse;
	execute(commandResponse);

	bool excludeSystem = _input->optValue("ExcludeSystem", false);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		std::string qName = (*it)->getParameterString(MQCA_AUTH_INFO_NAME);
		if ( excludeSystem
			&& qName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
