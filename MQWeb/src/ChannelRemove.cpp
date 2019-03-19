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
#include "MQ/Web/ChannelRemove.h"

namespace MQ {
namespace Web {

ChannelRemove::ChannelRemove(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: PCFSimpleCommand(commandServer, MQCMD_DELETE_CHANNEL, "Channel", input)
{
	// Required Parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");

	// Optional Parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	static TextMap channelTableMap = TextMapInitializer
		(MQCHTAB_Q_MGR, "QMgr")
		(MQCHTAB_CLNTCONN, "Client-connection")
	;
	std::string channelTable = input->get("ChannelTable");
	for (TextMap::const_iterator it = channelTableMap.begin(); it != channelTableMap.end(); ++it)
	{
		if (it->second.compare(channelTable) == 0) {
			pcf()->addParameter(MQIACH_CHANNEL_TABLE, it->first);
		}
	}
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	addParameterNumFromString(MQIACH_CHANNEL_TYPE, "ChannelType");
}

ChannelRemove::~ChannelRemove()
{
}

}} //  Namespace MQ::Web
