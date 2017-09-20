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
#include "MQ/Web/ClusterQueueManagerMapper.h"

namespace MQ {
namespace Web {


ClusterQueueManagerMapper::ClusterQueueManagerMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "ClusterQueueManager", input)
{
}

ClusterQueueManagerMapper::~ClusterQueueManagerMapper()
{
}


void ClusterQueueManagerMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void ClusterQueueManagerMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ClusterQueueManagerMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ClusterQueueManagerMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_CLUSTER_Q_MGR);

	// Required parameters
	addParameter<std::string>(MQCA_CLUSTER_Q_MGR_NAME, "ClusterQMgrName");

	// Optional parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");
	addParameter<std::string>(MQCA_CLUSTER_NAME, "ClusterName");
	addAttributeList(MQIACF_CLUSTER_Q_MGR_ATTRS, "ClusterQMgrAttrs");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addStringFilter();

	PCF::Vector commandResponse;
	execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

Poco::JSON::Object::Ptr ClusterQueueManagerMapper::suspend()
{
	createCommand(MQCMD_SUSPEND_Q_MGR_CLUSTER);

	// Required parameters
	addParameter<std::string>(MQCA_CLUSTER_NAME, "ClusterName");
	addParameter<std::string>(MQCA_CLUSTER_NAMELIST, "ClusterNamelist");

	// Optional parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addAttributeList(MQIACF_MODE, "Mode");

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

Poco::JSON::Object::Ptr ClusterQueueManagerMapper::resume()
{
	createCommand(MQCMD_RESUME_Q_MGR_CLUSTER);

	// Required parameters
	addParameter<std::string>(MQCA_CLUSTER_NAME, "ClusterName");
	addParameter<std::string>(MQCA_CLUSTER_NAMELIST, "ClusterNamelist");

	// Optional parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");

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
