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
#include "MQ/Web/SubscriptionMapper.h"

#include "Poco/HexBinaryDecoder.h"

namespace MQ {
namespace Web {


SubscriptionMapper::SubscriptionMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "Sub", input)
{
}

SubscriptionMapper::~SubscriptionMapper()
{
}


void SubscriptionMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void SubscriptionMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void SubscriptionMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr SubscriptionMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_SUBSCRIPTION);

	// Required parameters
	addParameter<std::string>(MQCACF_SUB_NAME, "SubName");

	if ( _input->has("SubId") )
	{
		std::string hexId = _input->get("SubId");
		if ( hexId.length() > MQ_CORREL_ID_LENGTH )
		{
			hexId.erase(MQ_CORREL_ID_LENGTH);
		}
		Buffer::Ptr id = new Buffer(hexId);

		pcf()->addParameter(MQBACF_SUB_ID, id);
	}

	// Optional parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addParameter<MQLONG>(MQIACF_DURABLE_SUBSCRIPTION, "Durable");
	addAttributeList(MQIACF_SUB_ATTRS, "SubscriptionAttrs");
	addParameterNumFromString(MQIACF_SUB_TYPE, "SubscriptionType");

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

		if ( excludeSystem )
		{
			std::string subName = (*it)->getParameterString(MQCACF_SUB_NAME);
			if ( subName.compare(0, 7, "SYSTEM.") == 0 )
			{
				continue;
			}
		}

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
