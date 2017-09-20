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
#include "MQ/Web/ChannelMapper.h"

namespace MQ {
namespace Web {


ChannelMapper::ChannelMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "Channel", input)
{
}

ChannelMapper::~ChannelMapper()
{
}


void ChannelMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void ChannelMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ChannelMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ChannelMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_CHANNEL);

	// Required Parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");

	// Optional Parameters
	addParameterNumFromString(MQIACH_CHANNEL_TYPE, "ChannelType");
	addAttributeList(MQIACF_CHANNEL_ATTRS, "ChannelAttrs");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addParameterNumFromString(MQIACH_CHANNEL_DISP, "DefaultChannelDisposition");
	addIntegerFilter();
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	addStringFilter();

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

		std::string channelName = (*it)->getParameterString(MQCACH_CHANNEL_NAME);
		if (    excludeSystem
			&& channelName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		json->add(createJSON(**it));
	}

	return json;
}

Poco::JSON::Object::Ptr ChannelMapper::start()
{
	createCommand(MQCMD_START_CHANNEL);

	// Required Parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");

	// Optional Parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addParameterNumFromString(MQIACH_CHANNEL_DISP, "ChannelDisposition");

	PCF::Vector commandResponse;
	execute(commandResponse);

	if ( commandResponse.size() > 0 )
	{
		PCF::Vector::iterator it = commandResponse.begin();
		if ( (*it)->getReasonCode() != MQRC_NONE )
		{
			return createError(**it);
		}
	}

	return new Poco::JSON::Object();
}

Poco::JSON::Object::Ptr ChannelMapper::stop()
{
	createCommand(MQCMD_STOP_CHANNEL);

	// Required Parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");

	// Optional Parameters
	addParameterNumFromString(MQIACH_CHANNEL_DISP, "ChannelDisposition");
	addParameterNumFromString(MQIACH_CHANNEL_STATUS, "ChannelStatus");
	addParameter<std::string>(MQCACH_CONNECTION_NAME, "ConnectionName");
	addParameterNumFromString(MQIACF_MODE, "Mode");
	addParameter<std::string>(MQCA_Q_MGR_NAME, "QMgrName");

	PCF::Vector commandResponse;
	execute(commandResponse);

	if ( commandResponse.size() > 0 )
	{
		PCF::Vector::iterator it = commandResponse.begin();
		if ( (*it)->getReasonCode() != MQRC_NONE )
		{
			return createError(**it);
		}
	}

	return new Poco::JSON::Object();
}

}} //  Namespace MQ::Web
