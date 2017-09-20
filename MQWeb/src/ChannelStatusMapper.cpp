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
#include "MQ/Web/ChannelStatusMapper.h"

namespace MQ {
namespace Web {


ChannelStatusMapper::ChannelStatusMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "ChannelStatus", input)
{
}

ChannelStatusMapper::~ChannelStatusMapper()
{
}


void ChannelStatusMapper::change()
{
  poco_assert_dbg(false); // Not applicable
}


void ChannelStatusMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not applicable
}


void ChannelStatusMapper::copy(bool replace)
{
  poco_assert_dbg(false); // Not applicable
}


Poco::JSON::Array::Ptr ChannelStatusMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_CHANNEL_STATUS);

	// Required parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");

	// Optional parameters
	addParameterNumFromString(MQIACH_CHANNEL_DISP, "ChannelDisposition");
	addParameter<std::string>(MQCACH_CLIENT_ID, "ClientIdentifier");
	addAttributeList(MQIACH_CHANNEL_INSTANCE_ATTRS, "ChannelInstanceAttrs");
	addParameterNumFromString(MQIACH_CHANNEL_INSTANCE_TYPE, "ChannelInstanceType");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addParameter<std::string>(MQCACH_CONNECTION_NAME, "ConnectionName");
	addIntegerFilter();
	addStringFilter();
	addParameter<std::string>(MQCACH_XMIT_Q_NAME, "XmitQName");

	PCF::Vector commandResponse;
	execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip Extended Response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
