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
#ifndef _MQWeb_MessageConsumerTask_INCLUDED
#define _MQWeb_MessageConsumerTask_INCLUDED

#include "Poco/Task.h"
#include "Poco/Net/WebSocket.h"

#include "MQ/QueueManagerPool.h"
#include "MQ/MessageConsumer.h"

namespace MQ {
namespace Web {

class MessageConsumerTask : public Poco::Task
{
public:
	MessageConsumerTask(Poco::SharedPtr<Poco::Net::WebSocket> ws, QueueManagerPoolGuard::Ptr queueManagerPoolGuard, const std::string& queueName, int limit = -1);

	virtual ~MessageConsumerTask();

	void cancel();

	void runTask();

	void onMessage(const void* pSender, Message::Ptr& msg);

	void onError(const void* pSender, MQLONG& rc);

private:
	Poco::SharedPtr<Poco::Net::WebSocket> _ws;

	QueueManagerPoolGuard::Ptr _qmgrPoolGuard;

	int _limit;

	Poco::SharedPtr<MessageConsumer> _consumer;

	int _count;

};

}} // Namespace MQ::Web

#endif // _MQWeb_MessageConsumerTask_INCLUDED
