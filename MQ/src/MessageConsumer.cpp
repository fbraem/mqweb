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
#include "Poco/Util/Application.h"

#include "MQ/MQSubsystem.h"
#include "MQ/MessageConsumer.h"
#include "MQ/Queue.h"

namespace MQ
{

MQGMO MessageConsumer::_initialGMO = { MQGMO_DEFAULT };

MessageConsumer::MessageConsumer(QueueManager& qmgr, const std::string& queueName, Action action)
	: _qmgr(qmgr)
	, _queue(qmgr, queueName)
	, _md(Message::_initialMD)
	, _action(action)
	, _gmo(MessageConsumer::_initialGMO)
	, _state(DIRTY)
{
	MQLONG options = MQOO_INPUT_SHARED | MQOO_FAIL_IF_QUIESCING;
	if ( action == BROWSE )
	{
		options |= MQOO_BROWSE;
	}
	_queue.open(options);
}

MessageConsumer::~MessageConsumer()
{
	try
	{
		if ( _state != DIRTY )
		{
			clear();
		}
	}
	catch(...)
	{
		// Don't throw in destructors
	}
}

void MessageConsumer::setup()
{
	MQCBD cbd = { MQCBD_DEFAULT };
	cbd.CallbackFunction = (MQPTR) MessageConsumer::consume;
	cbd.CallbackArea = this;

	if ( _action == BROWSE )
	{
		_gmo.Options |= MQGMO_BROWSE_NEXT;
	}

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	mqSystem.functions().cb(_qmgr.handle(), MQOP_REGISTER, &cbd, _queue.handle(), &_md, &_gmo);

	_state = READY;
}

void MessageConsumer::clear()
{
	MQCBD cbd = { MQCBD_DEFAULT };

	if ( _state == STARTED )
	{
		stop();
	}

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	mqSystem.functions().cb(_qmgr.handle(), MQOP_DEREGISTER, &cbd, _queue.handle(), &_md, &_gmo);

	_state = DIRTY;
}

void MessageConsumer::start()
{
	if ( _state == DIRTY )
	{
		setup();
	}

	// Start the consumer
	MQCTLO options = { MQCTLO_DEFAULT };

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	mqSystem.functions().ctl(_qmgr.handle(), MQOP_START, &options);

	_state = STARTED;
}

void MessageConsumer::stop()
{
	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	MQCTLO options = { MQCTLO_DEFAULT };
	mqSystem.functions().ctl(_qmgr.handle(), MQOP_STOP, &options);

	_state = STOPPED;
}

void MessageConsumer::consume(MQHCONN conn, MQMD* md, MQGMO* gmo, MQBYTE* buffer, MQCBC* context)
{
	Poco::SharedPtr<Message> msg = new Message(buffer, context->DataLength);
	memcpy(msg->md(), md, sizeof(MQMD));

	MessageConsumer* consumer = reinterpret_cast<MessageConsumer*>(context->CallbackArea);
	consumer->message.notify(consumer, msg);
}

}
