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

#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Task.h"

namespace MQ {
namespace Web {

class MessageConsumerTask : public Poco::Task, public MessageConsumer::Notifiable
{
public:
	MessageConsumerTask(Poco::SharedPtr<Poco::Net::WebSocket> ws, const std::string& qmgrName, const std::string& queueName) 
		: Poco::Task("MessageConsumer"), _ws(ws)
	{
		Poco::SharedPtr<QueueManagerPool> qmgrPool = QueueManagerPoolCache::instance()->getQueueManagerPool(qmgrName);
/*
		if ( qmgrPool.isNull() )
		{
			response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, "Out of memory: can't create a pool for queuemanager.");
			response.send();
			return;
		}
*/
		QueueManager::Ptr qmgr = qmgrPool->borrowObject();

/*
		if ( qmgr.isNull() )
		{
			response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, "No queuemanager available in the pool. Check the connection pool configuration.");
			response.send();
			return;
		}
*/
		_qmgrPoolGuard = new QueueManagerPoolGuard(qmgrPool, qmgr);

		_consumer = new MessageConsumer(*qmgr, queueName, this);
	}

	virtual ~MessageConsumerTask()
	{
	}

	void cancel()
	{
		_ws->close();
		_consumer->stop();

		Poco::Task::cancel();
	}

	void runTask()
	{
		_consumer->start();
		while(!isCancelled())
		{
			sleep(2000);
			std::cout << "Number of messages: so far ... " << _count << std::endl;
		}
	}

	void onMessage(const Message& msg)
	{
		_count++;
		std::string messageContent = msg.buffer().toString();
		_ws->sendFrame(messageContent.c_str(), messageContent.size(), Poco::Net::WebSocket::FRAME_TEXT);
	}

private:
	Poco::SharedPtr<Poco::Net::WebSocket> _ws;
	QueueManagerPoolGuard::Ptr _qmgrPoolGuard;
	Poco::SharedPtr<MessageConsumer> _consumer;

	static int _count;
};

int MessageConsumerTask::_count = 0;

WebSocketRequestHandler::WebSocketRequestHandler() : HTTPRequestHandler()
{
}

void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	std::string qmgrName = "EAGLE";

	try
	{
		Poco::SharedPtr<Poco::Net::WebSocket> ws = new Poco::Net::WebSocket(request, response);
		MQWebApplication& app = (MQWebApplication&) MQWebApplication::instance();
		app.taskManager().start(new MessageConsumerTask(ws, "EAGLE", "WEBSOCKET.Q01"));
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
