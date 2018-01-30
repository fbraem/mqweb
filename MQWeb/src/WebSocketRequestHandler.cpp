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
#include "MQ/Web/PCFCommand.h"
#include "MQ/Web/JSONMessage.h"
#include "MQ/Web/MQWebSubsystem.h"

#include "Poco/URI.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Task.h"

namespace MQ {
namespace Web {


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

	try
	{
		Poco::SharedPtr<Poco::Net::WebSocket> ws = new Poco::Net::WebSocket(request, response);
		MQWebSubsystem& mqWebsystem = Poco::Util::Application::instance().getSubsystem<MQWebSubsystem>();
		mqWebsystem.messageConsumerTaskManager().startTask(ws, paths[0], paths[1]);
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
	catch (Poco::OutOfMemoryException& oome)
	{
		response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, oome.message());
		response.setContentLength(0);
		response.send();
	}
	catch (MQ::MQException& mqex)
	{
		std::string msg("MQ RC=");
		Poco::NumberFormatter::append(msg, mqex.reason());
		msg += " - ";
		msg += PCFCommand::getReasonString(mqex.reason());
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, msg);
		response.setContentLength(0);
		response.send();
	}
}

}} // Namespace MQ::Web
