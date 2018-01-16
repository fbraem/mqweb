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
#include <sstream>

#include "MQ/Web/Application.h"
#include "MQ/Web/WebSocketRequestHandler.h"
#include "MQ/MessageConsumer.h"
#include "MQ/QueueManagerPool.h"
#include "MQ/Web/QueueManagerPoolCache.h"
#include "MQ/MQException.h"

#include "Poco/URI.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Task.h"

namespace MQ {
namespace Web {

class MessageConsumerTask : public Poco::Task
{
public:
	MessageConsumerTask(Poco::SharedPtr<Poco::Net::WebSocket> ws, QueueManagerPoolGuard::Ptr queueManagerPoolGuard, const std::string& queueName)
		: Poco::Task("MessageConsumer"), _ws(ws), _qmgrPoolGuard(queueManagerPoolGuard), _count(0)
	{
		poco_assert_dbg(! queueManagerPoolGuard.isNull());

		Poco::Timespan ts(600, 0);
		_ws->setReceiveTimeout(ts);
		_ws->setSendTimeout(ts);

		QueueManager::Ptr qmgr = queueManagerPoolGuard->getObject();
		_consumer = new MessageConsumer(qmgr, queueName, MessageConsumer::BROWSE);
		_consumer->message+= Poco::delegate(this, &MessageConsumerTask::onMessage);
	}

	virtual ~MessageConsumerTask()
	{
		_consumer->message-= Poco::delegate(this, &MessageConsumerTask::onMessage);
	}

	void cancel()
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");
		poco_debug(logger, "MessageConsumerTask cancelling ...");

		try
		{
			_consumer->stop();
		}
		catch(MQException&)
		{
			//Ignore to make sure the socket is closed and the task is cancelled.
		}

		_ws->close();

		Poco::Task::cancel();

		poco_debug(logger, "MessageConsumerTask cancelled.");
	}

	void runTask()
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");

		_consumer->start();

		char buffer[1024];
		int n;
		int flags;
		do
		{
			n = _ws->receiveFrame(buffer, sizeof(buffer), flags);
			poco_trace_f1(logger, "Number of messages: so far %d", _count);
		}
		while (n > 0 || (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
		poco_debug(logger, "WebSocket connection closed.");
	}

	void onMessage(const void* pSender, Poco::SharedPtr<Message>& msg)
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");
		poco_trace_f1(logger, "A message received %s", msg->messageId()->toHex());

		_count++;
		std::string messageContent = msg->buffer().toString();
		try
		{
			_ws->sendFrame(messageContent.c_str(), messageContent.size(), Poco::Net::WebSocket::FRAME_TEXT);
		}
		catch(Poco::Exception&)
		{
			cancel();
		}
	}

private:
	Poco::SharedPtr<Poco::Net::WebSocket> _ws;

	QueueManagerPoolGuard::Ptr _qmgrPoolGuard;

	Poco::SharedPtr<MessageConsumer> _consumer;

	int _count;

};

Poco::ThreadPool WebSocketRequestHandler::_tmThreadPool;

Poco::TaskManager WebSocketRequestHandler::_tm(WebSocketRequestHandler::_tmThreadPool);

WebSocketRequestHandler::WebSocketRequestHandler() : HTTPRequestHandler()
{
}

void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	Poco::URI uri(request.getURI());

	std::vector<std::string> paths;
	uri.getPathSegments(paths);

	if ( paths.size() < 2 )
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Missing path parameters");
		response.setContentLength(0);
		response.send();
		return;
	}

	Poco::SharedPtr<QueueManagerPool> qmgrPool = QueueManagerPoolCache::instance()->getQueueManagerPool(paths[0]);
	if ( qmgrPool.isNull() )
	{
		response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, "Out of memory: can't create a pool for queuemanager.");
		response.send();
		return;
	}

	QueueManager::Ptr qmgr = qmgrPool->borrowObject();
	if ( qmgr.isNull() )
	{
		response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, "No queuemanager available in the pool. Check the connection pool configuration.");
		response.send();
		return;
	}

	QueueManagerPoolGuard::Ptr qmgrPoolGuard = new QueueManagerPoolGuard(qmgrPool, qmgr);

	try
	{
		Poco::SharedPtr<Poco::Net::WebSocket> ws = new Poco::Net::WebSocket(request, response);
		MQWebApplication& app = (MQWebApplication&) MQWebApplication::instance();
		_tm.start(new MessageConsumerTask(ws, qmgrPoolGuard, paths[1]));
	}
	catch (Poco::Net::WebSocketException& exc)
	{
		switch (exc.code())
		{
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
			response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
			// fallthrough
		case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.setContentLength(0);
			response.send();
			break;
		}
	}
}

void WebSocketRequestHandler::cancel()
{
	// Cancel all launched tasks and wait for them to end ...
	_tm.cancelAll();
	_tm.joinAll();
}

}} // Namespace MQ::Web
