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
#include "MQ/Web/ChannelAuthenticationRecordSet.h"

namespace MQ {
namespace Web {


ChannelAuthenticationRecordSet::ChannelAuthenticationRecordSet(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
	: PCFCommand(commandServer, MQCMD_SET_CHLAUTH_REC, "ChannelAuthenticationRecord", input)
{
	// Required Parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");
	addParameterNumFromString(MQIACF_CHLAUTH_TYPE, "Type");

	fillPCF();
}

ChannelAuthenticationRecordSet::~ChannelAuthenticationRecordSet()
{
}


Poco::JSON::Array::Ptr ChannelAuthenticationRecordSet::execute()
{
	PCFCommand::execute();

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

	for(PCF::Vector::const_iterator it = begin(); it != end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web