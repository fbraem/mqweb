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

	MessageConsumer(QueueManager::Ptr qmgr, const std::string& queueName, Action action = BROWSE);
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

	typedef Poco::BasicEvent<Poco::SharedPtr<Message> > MessageEvent;
	MessageEvent messageEvent;

	typedef Poco::BasicEvent<MQLONG> ErrorEvent;
	ErrorEvent errorEvent;

private:

	static void consume(MQHCONN conn, MQMD* md, MQGMO* gmo, MQBYTE* buffer, MQCBC* context);
		/// Callback for MQCB

	QueueManager::Ptr _qmgr;

	Queue::Ptr _queue;

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
