/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
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

#include "Poco/Util/Application.h"

#include "MQ/MQSubsystem.h"
#include "MQ/MessageConsumer.h"
#include "MQ/Queue.h"

namespace MQ
{

MQGMO MessageConsumer::_initialGMO = { MQGMO_DEFAULT };

MessageConsumer::MessageConsumer(QueueManager& qmgr, const std::string& queueName, Notifiable* callee)
	: _qmgr(qmgr)
	, _queue(qmgr, queueName)
	, _md(Message::_initialMD)
	, _gmo(MessageConsumer::_initialGMO)
	, _state(DIRTY)
	, _callee(callee)
{
	_queue.open(MQOO_INPUT_SHARED | MQOO_FAIL_IF_QUIESCING);
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
	Message msg(buffer, context->DataLength);

	MessageConsumer* consumer = (MessageConsumer*)context->CallbackArea;
	consumer->_callee->onMessage(msg);
}

}
