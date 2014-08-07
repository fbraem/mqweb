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

AuthorityRecordMapper::AuthorityRecordMapper(CommandServer& commandServer) : MQMapper(commandServer, "AuthorityRecord")
{
}

AuthorityRecordMapper::~AuthorityRecordMapper()
{
}


void AuthorityRecordMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthorityRecordMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void AuthorityRecordMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr AuthorityRecordMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

	PCF::Ptr command = _commandServer.createCommand(MQCMD_INQUIRE_AUTH_RECS);
	
	if ( filter->has("profileName") ) command->addParameter(MQCACF_AUTH_PROFILE_NAME, filter->optValue<std::string>("profileName", "*"));
	
	MQLONG options = 0;
	Poco::JSON::Array::Ptr optionsValue = filter->getArray("options");
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
	}
	command->addParameter(MQIACF_AUTH_OPTIONS, options);

	std::string objectType = filter->optValue<std::string>("objectType", "All");
	MQLONG objectTypeValue = dictionary()->getDisplayId(MQIACF_OBJECT_TYPE, objectType);
	poco_assert_dbg(objectTypeValue != -1);
	if ( objectTypeValue == - 1 )
	{
		return json;
	}
	command->addParameter(MQIACF_OBJECT_TYPE, objectTypeValue);

	if ( filter->has("entityName") ) command->addParameter(MQCACF_ENTITY_NAME, filter->optValue<std::string>("entityName", ""));

	if ( filter->has("entityType") )
	{
		std::string entityType = filter->optValue<std::string>("entityType", "");
		if ( !entityType.empty() )
		{
			MQLONG entityTypeValue = dictionary()->getDisplayId(MQIACF_ENTITY_TYPE, entityType);
			poco_assert_dbg(entityTypeValue != -1);
			if ( entityTypeValue == - 1 )
			{
				return json;
			}
			command->addParameter(MQIACF_ENTITY_TYPE, entityTypeValue);
		}
	}
	
	if ( filter->has("serviceComponent") ) command->addParameter(MQCACF_SERVICE_COMPONENT, filter->optValue<std::string>("serviceComponent", ""));

	PCF::Vector commandResponse;
	_commandServer.sendCommand(command, commandResponse);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		Poco::JSON::Object::Ptr object = new Poco::JSON::Object();
		json->add(object);

		dictionary()->mapToJSON(**it, object);
	}

	return json;
}

}} //  Namespace MQ::Web
