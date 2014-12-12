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
		_consumer = new MessageConsumer(*qmgr, queueName);
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
		app.taskManager().start(new MessageConsumerTask(ws, qmgrPoolGuard, paths[1]));
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

}} // Namespace MQ::Web
