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
#include "MQ/Web/AuthorityServiceMapper.h"

namespace MQ {
namespace Web {

AuthorityServiceMapper::AuthorityServiceMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "AuthorityServiceMapper", input)
{
}

AuthorityServiceMapper::~AuthorityServiceMapper()
{
}


void AuthorityServiceMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthorityServiceMapper::create(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthorityServiceMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr AuthorityServiceMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_AUTH_SERVICE);

	// Required parameters
	addAttributeList(MQIACF_AUTH_SERVICE_ATTRS, "AuthServiceAttrs");
	
	// Optional parameters
	addParameter<std::string>(MQCACF_SERVICE_COMPONENT, "ServiceComponent");

	PCF::Vector commandResponse;
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
