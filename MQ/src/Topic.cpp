/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they
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
#include <string.h>

#include "Poco/Util/Application.h"

#include "MQ/MQSubsystem.h"
#include "MQ/Topic.h"
#include "MQ/QueueManager.h"
#include "MQ/MQException.h"
#include "MQ/Message.h"

namespace MQ
{

MQOD Topic::_initialOD = { MQOD_DEFAULT };

Topic::Topic(QueueManager& qmgr, const std::string& topic)
	: _qmgr(qmgr)
	, _handle(0)
	, _od(_initialOD)
	, _topic(topic)
{
	_od.ObjectType = MQOT_TOPIC;
	strncpy(_od.ObjectName, topic.c_str(), MQ_Q_NAME_LENGTH);
	_od.Version = MQOD_VERSION_4;
}

Topic::Topic(QueueManager& qmgr, const std::string& topic, const std::string& topicStr) 
	: _qmgr(qmgr)
	, _handle(0)
	, _od(_initialOD)
	, _topic(topic)
	, _topicStr(topicStr)
{
	_od.ObjectType = MQOT_TOPIC;
	strncpy(_od.ObjectName, topic.c_str(), MQ_Q_NAME_LENGTH);
	_od.Version = MQOD_VERSION_4;

	_od.ObjectString.VSPtr = (MQPTR) _topicStr.c_str();
	_od.ObjectString.VSLength = (MQLONG) _topicStr.size();
}

Topic::~Topic()
{
	if ( _handle != 0 )
	{
		try
		{
			close();
		}
		catch(MQException&)
		{
			// Don't throw exceptions from a destructor
			// See: http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.9
		}
	}
}

void Topic::open(long options)
{
	char resObjectString[MQ_TOPIC_STR_LENGTH];
	memset(resObjectString, 0, MQ_TOPIC_STR_LENGTH);
	_od.ResObjectString.VSPtr = resObjectString;
	_od.ResObjectString.VSBufSize = MQ_TOPIC_STR_LENGTH;

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	_handle = mqSystem.functions().open(_qmgr.handle(), &_od, options);

	_resObjectStr = resObjectString;
	_resObjectStr = Poco::trimRightInPlace(_resObjectStr);
}

void Topic::close()
{
	if ( _handle != 0 )
	{
		MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
		mqSystem.functions().close(_qmgr.handle(), &_handle, MQCO_NONE);
	}
}

void Topic::publish(Message& msg, MQLONG options)
{
	MQPMO pmo = { MQPMO_DEFAULT };
	pmo.Options = options;

	MQLONG size = (MQLONG) msg.buffer().size();

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	try
	{
		mqSystem.functions().put(_qmgr.handle(), _handle, msg.md(), &pmo, size, size > 0 ? msg.buffer().data() : NULL);
	}
	catch(MQException& mqe)
	{
		mqe.object(_topic);
		throw;
	}
}

void Topic::inquire(const std::vector<int>& intSelectors, const std::map<int, int>& charSelectors, std::map<int, int>& intResult, std::map<int, std::string>& charResult)
{
	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	try
	{
		mqSystem.functions().inq(_qmgr.handle(), _handle, intSelectors, charSelectors, intResult, charResult);
	}
	catch(MQException& mqe)
	{
		mqe.object(_topic);
		throw;
	}
}

}
