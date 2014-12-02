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
#include "MQ/Queue.h"
#include "MQ/QueueManager.h"
#include "MQ/MQException.h"
#include "MQ/Message.h"

namespace MQ
{

MQOD Queue::_initialOD = { MQOD_DEFAULT };

Queue::Queue(QueueManager& qmgr, const std::string& name)
	: _qmgr(qmgr)
	, _handle(0)
	, _od(_initialOD)
{
	strncpy(_od.ObjectName, name.c_str(), MQ_Q_NAME_LENGTH);
}

Queue::~Queue()
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

void Queue::open(long options)
{
	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	_handle = mqSystem.functions().open(_qmgr.handle(), &_od, options);
}

void Queue::close()
{
	if ( _handle != 0 )
	{
		MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
		mqSystem.functions().close(_qmgr.handle(), &_handle, MQCO_NONE);
	}
}

void Queue::put(Message& msg, MQLONG options)
{
	MQPMO pmo = { MQPMO_DEFAULT };
	pmo.Options = options;

	int size = msg.buffer().size();

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	try
	{
		mqSystem.functions().put(_qmgr.handle(), _handle, msg.md(), &pmo, size, size > 0 ? msg.buffer().data() : NULL);
	}
	catch(MQException& mqe)
	{
		mqe.object(_od.ObjectName);
		throw;
	}
}


void Queue::get(Message& msg, MQLONG options, long wait)
{
	MQGMO gmo = { MQGMO_DEFAULT };

	if ( ! msg.messageId()->hasAllNullBytes() )
	{
		gmo.MatchOptions |= MQMO_MATCH_MSG_ID;
	}

	if ( ! msg.correlationId()->hasAllNullBytes() )
	{
		gmo.MatchOptions |= MQMO_MATCH_CORREL_ID;
	}

	if ( wait == -1 )
	{
		gmo.Options |= MQGMO_WAIT;
		gmo.WaitInterval = MQWI_UNLIMITED;
	}
	else if ( wait > 0 )
	{
		gmo.Options |= MQGMO_WAIT;
		gmo.WaitInterval = wait;
	}
	gmo.Options |= options;

	int size = msg.buffer().size();
	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	try
	{
		mqSystem.functions().get(_qmgr.handle(), _handle, msg.md(), &gmo, size, size > 0 ? msg.buffer().data() : NULL, &msg._dataLength);
	}
	catch(MQException& mqe)
	{
		mqe.object(_od.ObjectName);
		throw;
	}
}

void Queue::inquire(const std::vector<int>& intSelectors, const std::map<int, int>& charSelectors, std::map<int, int>& intResult, std::map<int, std::string>& charResult)
{
	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	try
	{
		mqSystem.functions().inq(_qmgr.handle(), _handle, intSelectors, charSelectors, intResult, charResult);
	}
	catch(MQException& mqe)
	{
		mqe.object(_od.ObjectName);
		throw;
	}
}

}
