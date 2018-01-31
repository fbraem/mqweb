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
#include "Poco/Delegate.h"
#include "Poco/Logger.h"
#include "Poco/Net/NetException.h"

#include "MQ/MQException.h"
#include "MQ/Web/MessageConsumerTask.h"
#include "MQ/Web/JSONMessage.h"

namespace MQ {
namespace Web {

MessageConsumerTask::MessageConsumerTask(Poco::SharedPtr<Poco::Net::WebSocket> ws, QueueManagerPoolGuard::Ptr queueManagerPoolGuard, const std::string& queueName)
	: Poco::Task("MessageConsumer"), _ws(ws), _qmgrPoolGuard(queueManagerPoolGuard), _count(0)
{
	poco_assert_dbg(!queueManagerPoolGuard.isNull());

	Poco::Timespan ts(10, 0);
	_ws->setReceiveTimeout(ts);
	_ws->setSendTimeout(ts);

	QueueManager::Ptr qmgr = queueManagerPoolGuard->getObject();
	_consumer = new MessageConsumer(qmgr, queueName, MessageConsumer::BROWSE);
	_consumer->messageEvent += Poco::delegate(this, &MessageConsumerTask::onMessage);
	_consumer->errorEvent += Poco::delegate(this, &MessageConsumerTask::onError);
}

MessageConsumerTask::~MessageConsumerTask()
{
	_consumer->messageEvent -= Poco::delegate(this, &MessageConsumerTask::onMessage);
	_consumer->errorEvent -= Poco::delegate(this, &MessageConsumerTask::onError);
}

void MessageConsumerTask::cancel()
{
	Poco::Logger& logger = Poco::Logger::get("mq.web");
	logger.trace("MessageConsumerTask cancelling ...");

	try
	{
		_consumer->stop();
	}
	catch (MQException&)
	{
		//Ignore to make sure the socket is closed and the task is cancelled.
	}

	_ws->close();

	Poco::Task::cancel();

	logger.trace("MessageConsumerTask cancelled.");
}

void MessageConsumerTask::runTask()
{
	Poco::Logger& logger = Poco::Logger::get("mq.web");
	logger.trace("MessageConsumerTask started ...");

	_consumer->start();

	char buffer[1024];
	int n;
	int flags;
	do
	{
		try
		{
			n = _ws->receiveFrame(buffer, sizeof(buffer), flags);
			logger.trace("Number of messages: so far %d", _count);
		}
		catch (Poco::TimeoutException&)
		{
			logger.trace("Timeout");
			break;
		}
		catch (Poco::Net::NetException&)
		{
			logger.trace("NetException received. WebSocket closed by the client?");
			break;
		}
	} while (n > 0 || (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);

	logger.trace("MessageConsumerTask ended.");
}

void MessageConsumerTask::onMessage(const void* pSender, Message::Ptr& msg)
{
	Poco::Logger& logger = Poco::Logger::get("mq.web");
	logger.trace("A message is received %s", msg->messageId()->toHex());

	_count++;

	JSONMessage jsonMessage(msg);
	Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
	jsonMessage.toJSON(json);

	std::stringstream ss;
	json->stringify(ss);
	std::string content = ss.str();
	try
	{
		_ws->sendFrame(content.c_str(), content.length(), Poco::Net::WebSocket::FRAME_TEXT);
	}
	catch (Poco::Exception&)
	{
		cancel();
	}
}

void MessageConsumerTask::onError(const void* pSender, MQLONG& rc)
{
	Poco::Logger& logger = Poco::Logger::get("mq.web");
	logger.trace("An MQ error is received %ld. Task will be cancelled.", rc);
	cancel();
}

}} // Namespace MQ::Web
