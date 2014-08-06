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
#include "MQ/Web/Dictionary.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {

AuthenticationInformationMapper::AuthenticationInformationMapper(CommandServer& commandServer) : MQMapper(commandServer, "AuthenticationInformation")
{
}

AuthenticationInformationMapper::~AuthenticationInformationMapper()
{
}


void AuthenticationInformationMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthenticationInformationMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthenticationInformationMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr AuthenticationInformationMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr jsonAuthInfos = new Poco::JSON::Array();

	PCF::Ptr command = _commandServer.createCommand(MQCMD_INQUIRE_Q);
	command->addParameter(MQCA_AUTH_INFO_NAME, filter->optValue<std::string>("authInfoName", "*"));

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

		std::string qName = (*it)->getParameterString(MQCA_AUTH_INFO_NAME);
		if ( excludeSystem
			&& qName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		Poco::JSON::Object::Ptr jsonAuthInfo = new Poco::JSON::Object();
		jsonAuthInfos->add(jsonAuthInfo);

		dictionary()->mapToJSON(**it, jsonAuthInfo);
	}

	return jsonAuthInfos;
}

}} //  Namespace MQ::Web
