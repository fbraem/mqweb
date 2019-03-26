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
#include "MQ/MQException.h"
#include "MQ/Web/ChannelStop.h"

namespace MQ {
namespace Web {


ChannelStop::ChannelStop(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: PCFCommand(commandServer, MQCMD_STOP_CHANNEL, "Channel", input)
{
	// Required Parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");

	// Optional Parameters
	addParameterNumFromString(MQIACH_CHANNEL_DISP, "ChannelDisposition");
	addParameterNumFromString(MQIACH_CHANNEL_STATUS, "ChannelStatus");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addParameter<std::string>(MQCACH_CONNECTION_NAME, "ConnectionName");
	addParameterNumFromString(MQIACF_MODE, "Mode");
	addParameter<std::string>(MQCA_Q_MGR_NAME, "QMgrName");
}

ChannelStop::~ChannelStop()
{
}

Poco::JSON::Array::Ptr ChannelStop::execute()
{
	PCFCommand::execute();

	if ( hasResponse() )
	{
		PCF::Vector::const_iterator it = begin();
		if ( (*it)->getReasonCode() != MQRC_NONE )
		{
			throw MQException("PCF", getCommandString((*it)->getCommand()), (*it)->getCompletionCode(), (*it)->getReasonCode());
		}
	}

	return new Poco::JSON::Array();
}

}} //  Namespace MQ::Web
