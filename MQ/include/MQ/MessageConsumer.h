/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
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

#ifndef _MQ_MessageConsumer_H
#define _MQ_MessageConsumer_H

#include <cmqc.h>

#include "MQ/QueueManager.h"
#include "MQ/Message.h"

namespace MQ
{

class MessageConsumer
	/// Implemenst a message consumer.
	/// Note: the message buffer is only valid (the Message class doesn't
	/// own it) when used in the callback. If you need it after this 
	/// call, you need to copy it!
{
public:

	enum Action
	{
		BROWSE,
		GET
	};

	MessageConsumer(QueueManager& qmgr, const std::string& queueName, Action action = BROWSE);
		/// Constructor. Can throw a MQException.

	virtual ~MessageConsumer();
		/// Destructor. The callback will be cleared when this is not done yet.

	Action action() const;

	void clear();
		/// Deregisters the consumer. When a consumer is active, it will be stopped.
		/// Can throw a MQException.

	Buffer::Ptr correlId();

	Buffer::Ptr messageId();

	void setup();
		/// Registers the consumer. Can throw a MQException.
		/// When this is not called before calling start, it will be called
		/// automatically.

	void start();
		/// Starts the consumer. Can throw a MQException.

	enum State
	{
		DIRTY,
		READY,
		STARTED,
		STOPPED,
		SUSPENDED
	};

	State state() const;

	void stop();
		/// Stops the consumer. Can throw a MQException.

	typedef Poco::BasicEvent<Poco::SharedPtr<Message> > Event;

	Event message;

private:

	static void consume(MQHCONN conn, MQMD* md, MQGMO* gmo, MQBYTE* buffer, MQCBC* context);
		/// Callback for MQCB

	QueueManager& _qmgr;

	Queue _queue;

	Action _action;

	MQMD _md;

	MQGMO _gmo;

	State _state;

	static MQGMO _initialGMO;
};

inline MessageConsumer::Action MessageConsumer::action() const
{
	return _action;
}

inline Buffer::Ptr MessageConsumer::messageId()
{
	return new Buffer(_md.MsgId, MQ_MSG_ID_LENGTH);
}

inline Buffer::Ptr MessageConsumer::correlId()
{
	return new Buffer(_md.CorrelId, MQ_CORREL_ID_LENGTH);
}

inline MessageConsumer::State MessageConsumer::state() const
{
	return _state;
}

} // namespace QM

#endif // _MQ_MessageConsumer_H
