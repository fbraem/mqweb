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
#include "MQ/Web/AuthorityRecordMapper.h"

namespace MQ {
namespace Web {

AuthorityRecordMapper::AuthorityRecordMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "AuthorityRecord", input)
{
}


AuthorityRecordMapper::~AuthorityRecordMapper()
{
}


void AuthorityRecordMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthorityRecordMapper::create(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthorityRecordMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr AuthorityRecordMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_AUTH_RECS);

	// Required parameters
	MQLONG options = 0;
	Poco::JSON::Array::Ptr optionsValue = _input->getArray("Options");
	if ( !optionsValue.isNull() )
	{
		for(Poco::JSON::Array::ValueVec::const_iterator it = optionsValue->begin(); it != optionsValue->end(); ++it)
		{
			std::string value = *it;
			if ( value.compare("Name All Matching") == 0 )
			{
				options |= MQAUTHOPT_NAME_ALL_MATCHING;
			}
			else if ( value.compare("Name Explicit") == 0 )
			{
				options |= MQAUTHOPT_NAME_EXPLICIT;
			}
			else if ( value.compare("Entity Explicit") == 0 )
			{
				options |= MQAUTHOPT_ENTITY_EXPLICIT;
			}
			else if ( value.compare("Entity Set") == 0 )
			{
				options |= MQAUTHOPT_ENTITY_SET;
			}
			else if ( value.compare("Name As Wildcard") == 0 )
			{
				options |= MQAUTHOPT_NAME_AS_WILDCARD;
			}
		}
		pcf()->addParameter(MQIACF_AUTH_OPTIONS, options);
	}
	addParameter<std::string>(MQCACF_AUTH_PROFILE_NAME, "ProfileName");
	addParameterNumFromString(MQIACF_OBJECT_TYPE, "ObjectType");

	// Optional parameters
	addParameter<std::string>(MQCACF_ENTITY_NAME, "EntityName");
	addParameterNumFromString(MQIACF_ENTITY_TYPE, "EntityType");
	addAttributeList(MQIACF_AUTH_PROFILE_ATTRS, "ProfileAttrs");
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
