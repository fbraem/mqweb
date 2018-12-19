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
#include <string.h>

#include "Poco/Util/Application.h"

#include "MQ/MQSubsystem.h"
#include "MQ/QueueManager.h"
#include "MQ/Queue.h"
#include "MQ/MQException.h"
#include "MQ/Message.h"

namespace MQ
{

MQOD Queue::_initialOD = { MQOD_DEFAULT };

Queue::Queue(QueueManager::Ptr qmgr, const std::string& name)
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
	_handle = mqSystem.functions().open(_qmgr->handle(), &_od, options);
}

void Queue::close()
{
	if ( _handle != 0 )
	{
		MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
		mqSystem.functions().close(_qmgr->handle(), &_handle, MQCO_NONE);
	}
}

void Queue::put(Message& msg, MQLONG options)
{
	MQPMO pmo = { MQPMO_DEFAULT };
	pmo.Options = options;

	size_t size = msg.buffer().size();

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	try
	{
		mqSystem.functions().put(_qmgr->handle(), _handle, msg.md(), &pmo, (MQLONG) size, size > 0 ? msg.buffer().data() : NULL);
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

	size_t size = msg.buffer().size();
	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	try
	{
		mqSystem.functions().get(_qmgr->handle(), _handle, msg.md(), &gmo, (MQLONG) size, size > 0 ? msg.buffer().data() : NULL, &msg._dataLength);
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
		mqSystem.functions().inq(_qmgr->handle(), _handle, intSelectors, charSelectors, intResult, charResult);
	}
	catch(MQException& mqe)
	{
		mqe.object(_od.ObjectName);
		throw;
	}
}

}
