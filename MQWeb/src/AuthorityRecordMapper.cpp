/*
* Copyright 2017 - KBC Group NV - Franky Braem - The MIT license
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
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
	// When no ProfileName is passed, set to empty string
	if ( !_input->has("ProfileName") ) _input->set("ProfileName", "");
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
