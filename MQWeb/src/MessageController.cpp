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
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Poco/DateTimeFormatter.h"
#include "Poco/Logger.h"
#include "Poco/Timezone.h"

#include "MQ/Web/MessageController.h"
#include "MQ/Web/PCFCommand.h"
#include "MQ/Web/JSONMessage.h"
#include "MQ/MQException.h"
#include "MQ/PCF.h"
#include "MQ/Message.h"
#include "MQ/QueueManager.h"
#include "MQ/Queue.h"
#include "MQ/Topic.h"
#include "MQ/Buffer.h"

static unsigned char EBCDIC_translate_ASCII[256] =
{
	0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
	0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
	0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2F,
	0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
	0x20, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x3C, 0x28, 0x2B, 0x7C,
	0x26, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x21, 0x24, 0x2A, 0x29, 0x3B, 0x5E,
	0x2D, 0x2F, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x7C, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
	0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
	0x2E, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
	0x2E, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
	0x2E, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x2E, 0x2E, 0x2E, 0x5B, 0x2E, 0x2E,
	0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x5D, 0x2E, 0x2E,
	0x7B, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
	0x7D, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
	0x5C, 0x2E, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E
};

#define DEFAULT_EVENT_MESSAGE_SIZE 512

namespace MQ {
namespace Web {


MessageController::MessageController() : MQController()
{
}


MessageController::~MessageController()
{
}

void MessageController::get()
{
	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter is queuename
	if (parameters.size() < 2)
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Missing URI parameters");
		return;
	}

	std::string queueName = parameters[1];
	meta().set("queue", queueName);

	int limit = -1;

	std::string messageId;
	if (parameters.size() > 2)
	{
		limit = 1;
		messageId = parameters[2];
		meta().set("messageId", messageId);
	}
	else
	{
		std::string limitField = form().get("limit", "");
		if (!limitField.empty())
		{
			Poco::NumberParser::tryParse(limitField, limit);
		}
	}

	int maxMessageSize = 1024;
	std::string sizeField = form().get("size", "");
	if (!sizeField.empty())
	{
		Poco::NumberParser::tryParse(sizeField, maxMessageSize);
	}

	Poco::JSON::Array::Ptr jsonMessages = new Poco::JSON::Array();

	Queue q(qmgr(), queueName);
	q.open(MQOO_INPUT_SHARED);

	int count = 0;
	while (limit == -1 || count < limit)
	{
		Message::Ptr msg = new Message(maxMessageSize);
		if (!messageId.empty())
		{
			msg->messageId()->fromHex(messageId);
		}

		try
		{
			q.get(*msg, MQGMO_PROPERTIES_FORCE_MQRFH2, 0);
		}
		catch (MQException& mqe)
		{
			std::string currentMessageId = msg->messageId()->toHex();
			if (mqe.reason() == MQRC_NO_MSG_AVAILABLE)
			{
				if (!messageId.empty()) throw;
				break;
			}
			else if (mqe.reason() == MQRC_TRUNCATED_MSG_FAILED)
			{
				if (msg->getFormat().compare(MQFMT_EVENT) == 0
					|| msg->getFormat().compare(MQFMT_ADMIN) == 0) {
					// ignore size limit for event/admin messages and retry to get it with the
					// real length
					msg->buffer().resize(msg->dataLength(), false);
					msg->clear();
					msg->messageId()->fromHex(currentMessageId);

					try
					{
						q.get(*msg, MQGMO_PROPERTIES_FORCE_MQRFH2 | MQGMO_ACCEPT_TRUNCATED_MSG);
					}
					catch (MQException& mqe2)
					{
						if (mqe2.reason() != MQRC_TRUNCATED_MSG_ACCEPTED)
						{
							throw;
						}
					}
				}
				else if (msg->getFormat().compare(MQFMT_XMIT_Q_HEADER) == 0)
				{
					// ignore size limit for xmitq messages and retry to get it with at least the
					// length of the xmitq header
					msg->buffer().resize(sizeof(MQXQH), false);
					msg->clear();
					msg->messageId()->fromHex(currentMessageId);
					try
					{
						q.get(*msg, MQGMO_PROPERTIES_FORCE_MQRFH2 | MQGMO_ACCEPT_TRUNCATED_MSG);
					}
					catch (MQException& mqe2)
					{
						if (mqe2.reason() != MQRC_TRUNCATED_MSG_ACCEPTED)
						{
							throw;
						}
					}
				}
				else if (msg->getFormat().compare(MQFMT_DEAD_LETTER_HEADER) == 0)
				{
					// ignore size limit for dlh messages and retry to get it with at least the
					// length of the dlh header
					msg->buffer().resize(sizeof(MQDLH), false);
					msg->clear();
					msg->messageId()->fromHex(currentMessageId);
					try
					{
						q.get(*msg, MQGMO_PROPERTIES_FORCE_MQRFH2 | MQGMO_ACCEPT_TRUNCATED_MSG);
					}
					catch (MQException& mqe2)
					{
						if (mqe2.reason() != MQRC_TRUNCATED_MSG_ACCEPTED)
						{
							throw;
						}
					}
				}
				else
				{
					MQLONG size = msg->dataLength();
					msg->buffer().resize(size);
					msg->messageId()->fromHex(currentMessageId);
					try
					{
						q.get(*msg, MQGMO_PROPERTIES_FORCE_MQRFH2 | MQGMO_ACCEPT_TRUNCATED_MSG);
					}
					catch (MQException& mqe2)
					{
						if (mqe2.reason() != MQRC_TRUNCATED_MSG_ACCEPTED)
						{
							throw;
						}
					}
				}
			}
			else
			{
				throw;
			}
		}

		count++;

		Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

		JSONMessage jsonMessage(msg);
		json->set("mqmd", jsonMessage.toJSONMQMD());
		jsonMessage.toJSON(json);
		jsonMessages->add(json);
	}

	setData("data", jsonMessages);
}

void MessageController::browse()
{
	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter is queuename
	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Missing URI parameters");
		return;
	}

	std::string queueName = parameters[1];
	meta().set("queue", queueName);

	int limit = -1;

	std::string messageId;
	if ( parameters.size() > 2 )
	{
		limit = 1;
		messageId = parameters[2];
		meta().set("messageId", messageId);
	}
	else
	{
		std::string limitField = form().get("limit", "");
		if ( ! limitField.empty() )
		{
			Poco::NumberParser::tryParse(limitField, limit);
		}
	}

	int maxMessageSize = 1024;
	std::string sizeField = form().get("size", "");
	if (!sizeField.empty())
	{
		Poco::NumberParser::tryParse(sizeField, maxMessageSize);
	}

	Poco::JSON::Array::Ptr jsonMessages = new Poco::JSON::Array();

	Queue q(qmgr(), queueName);
	q.open(MQOO_BROWSE);

	int count = 0;
	while(limit == -1 || count < limit)
	{
		Message::Ptr msg = new Message(maxMessageSize);
		if ( ! messageId.empty() )
		{
			msg->messageId()->fromHex(messageId);
		}

		try
		{
			q.get(*msg, MQGMO_BROWSE_NEXT | MQGMO_PROPERTIES_FORCE_MQRFH2, 0);
		}
		catch(MQException& mqe)
		{
			if ( mqe.reason() == MQRC_NO_MSG_AVAILABLE )
			{
				if (! messageId.empty()) throw;
				break;
			}
			else if ( mqe.reason() == MQRC_TRUNCATED_MSG_FAILED )
			{
				if ( msg->getFormat().compare(MQFMT_EVENT) == 0 
					|| msg->getFormat().compare(MQFMT_ADMIN) == 0 ) {
					// ignore size limit for event/admin messages and retry to get it with the
					// real length
					msg->buffer().resize(msg->dataLength(), false);
					msg->clear();
					try
					{
						q.get(*msg, MQGMO_BROWSE_NEXT | MQGMO_PROPERTIES_FORCE_MQRFH2 | MQGMO_ACCEPT_TRUNCATED_MSG);
					}
					catch(MQException& mqe2)
					{
						if ( mqe2.reason() != MQRC_TRUNCATED_MSG_ACCEPTED )
						{
							throw;
						}
					}
				}
				else if ( msg->getFormat().compare(MQFMT_XMIT_Q_HEADER) == 0 )
				{
					// ignore size limit for xmitq messages and retry to get it with at least the
					// length of the xmitq header
					msg->buffer().resize(sizeof(MQXQH), false);
					msg->clear();
					try
					{
						q.get(*msg, MQGMO_BROWSE_NEXT | MQGMO_PROPERTIES_FORCE_MQRFH2 | MQGMO_ACCEPT_TRUNCATED_MSG);
					}
					catch(MQException& mqe2)
					{
						if ( mqe2.reason() != MQRC_TRUNCATED_MSG_ACCEPTED )
						{
							throw;
						}
					}
				}
				else if ( msg->getFormat().compare(MQFMT_DEAD_LETTER_HEADER) == 0 )
				{
					// ignore size limit for dlh messages and retry to get it with at least the
					// length of the dlh header
					msg->buffer().resize(sizeof(MQDLH), false);
					msg->clear();
					try
					{
						q.get(*msg, MQGMO_BROWSE_NEXT | MQGMO_PROPERTIES_FORCE_MQRFH2 | MQGMO_ACCEPT_TRUNCATED_MSG);
					}
					catch(MQException& mqe2)
					{
						if ( mqe2.reason() != MQRC_TRUNCATED_MSG_ACCEPTED )
						{
							throw;
						}
					}
				}
				else
				{
					//TODO: do we try a resize?
					throw;
				}
			}
			else
			{
				throw;
			}
		}

		count++;

		Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

		JSONMessage jsonMessage(msg);
		json->set("mqmd", jsonMessage.toJSONMQMD());
		jsonMessage.toJSON(json);
		jsonMessages->add(json);
	}

	setData("data", jsonMessages);
}


void MessageController::dump()
{
	std::vector<std::string> parameters = getParameters();

	if ( parameters.size() < 3 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string queueName = parameters[1];
	meta().set("queue", queueName);

	std::string messageId = parameters[2];
	meta().set("messageId", messageId);

	Message::Ptr message = new Message();

	try
	{
		message->messageId()->fromHex(messageId);
	}
	catch(Poco::DataFormatException&)
	{
		//An invalid message id is passed (No valid HEX character)
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "An invalid message id is passed (No valid HEX character)");
		return;
	}

	Queue q(qmgr(), queueName);
	q.open(MQOO_BROWSE);

	try
	{
		q.get(*message, MQGMO_BROWSE_FIRST);
		message->buffer().resize(message->dataLength());
	}
	catch(MQException& mqe)
	{
		if ( mqe.reason()   == MQRC_TRUNCATED_MSG_FAILED
			|| mqe.reason() == MQRC_TRUNCATED )
		{
			MQLONG size = message->dataLength();
			//if ( size > 1024 * 16) size = 1024 * 16;
			message = new Message(size);
			message->messageId()->fromHex(messageId);

			try
			{
				q.get(*message, MQGMO_BROWSE_FIRST);
			}
			catch(MQException& mqe)
			{
				if ( mqe.reason()   != MQRC_TRUNCATED_MSG_FAILED
					&& mqe.reason() != MQRC_TRUNCATED )
				{
					throw;
				}
			}
		}
		else
		{
			throw;
		}
	}

	Poco::JSON::Object::Ptr jsonMessage = new Poco::JSON::Object();

	Poco::JSON::Array::Ptr jsonDump = new Poco::JSON::Array();
	jsonMessage->set("dump", jsonDump);

	std::string fullHex = message->buffer().toHex();
	Poco::JSON::Object::Ptr jsonMessageDump = new Poco::JSON::Object();
	jsonMessageDump->set("position", "00000000");
	jsonDump->add(jsonMessageDump);

	std::string hexPart;
	for(int i = 0; i < fullHex.length(); ++i)
	{
		if ( i && i % 2 == 0 && i % 16 != 0 )
		{
			hexPart += ' ';
		}

		if ( i && i % 16 == 0 && i % 32 != 0 )
		{
			hexPart += "--";
		}

		if ( i && i % 32 == 0 )
		{
			jsonMessageDump->set("hex", hexPart);
			hexPart.clear();

			jsonMessageDump = new Poco::JSON::Object();
			std::ostringstream outputPos;
			outputPos << std::uppercase << std::right << std::setw(8) << std::setfill('0') << std::setbase(16) << (i / 2);
			jsonMessageDump->set("position", outputPos.str());
			jsonDump->add(jsonMessageDump);
		}

		hexPart += fullHex[i];
	}
	jsonMessageDump->set("hex", hexPart);

	std::ostringstream oss;
	int row = 0;
	for(int i = 0; i < message->buffer().size(); ++i)
	{
		unsigned char code = message->buffer().chr(i);
		oss << (char) (message->getCodedCharSetId() == 500 ? EBCDIC_translate_ASCII[code] : code);
		if ( (i + 1) % 16 == 0 )
		{
			jsonMessageDump = jsonDump->getObject(row++);
			if ( !jsonMessageDump.isNull() )
			{
				jsonMessageDump->set("ascii", oss.str());
				oss.str("");
			}
		}
	}
	jsonMessageDump = jsonDump->getObject(row);
	if ( !jsonMessageDump.isNull() )
	{
		jsonMessageDump->set("ascii", oss.str());
	}
	
	JSONMessage jsonMessageConverter(message);
	jsonMessage->set("mqmd", jsonMessageConverter.toJSONMQMD());

	setData("data", jsonMessage);
}


void MessageController::publish()
{
	std::vector<std::string> parameters = getParameters();

	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string topicName = parameters[1];

	Poco::SharedPtr<Topic> topic;
	std::string topicStr;
	if ( parameters.size() > 2 )
	{
		topic = new Topic(qmgr(), topicName, parameters[2]);
	}
	else
	{
		topic = new Topic(qmgr(), topicName);
	}

	std::string topicString;
	if ( parameters.size() > 2 )
	{
		topicString = parameters[2];
	}

	Message::Ptr message = new Message();

	Poco::JSON::Object::Ptr input = data().getObject("input");
	if ( input.isNull() )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "JSON object missing");
		return;
	}

	MQLONG openOptions = MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING | MQBND_BIND_ON_OPEN;
	MQLONG publishOptions = 0;
	Poco::JSON::Object::Ptr md = input->getObject("mqmd");
	if ( ! md.isNull() )
	{
		JSONMessage jsonMessage(message);
		jsonMessage.toMQMD(*md);
		openOptions |= MQOO_SET_ALL_CONTEXT;
		publishOptions |= MQPMO_SET_ALL_CONTEXT;
	}
	topic->open(openOptions);

	std::string d = input->getValue<std::string>("data");
	message->setFormat("MQSTR");
	message->buffer().resize(d.length());
	memcpy(message->buffer().data(), d.c_str(), d.length());

	topic->publish(*message, publishOptions);
}

void MessageController::put()
{
	std::vector<std::string> parameters = getParameters();

	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string qName = parameters[1];

	Message::Ptr message = new Message();

	Poco::JSON::Object::Ptr input = data().getObject("input");
	if ( input.isNull() )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "JSON object missing");
		return;
	}

	MQLONG openOptions = MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING | MQBND_BIND_ON_OPEN;
	MQLONG putOptions = 0;
	Poco::JSON::Object::Ptr md = input->getObject("mqmd");
	if ( ! md.isNull() )
	{
		JSONMessage jsonMessage(message);
		jsonMessage.toMQMD(*md);
		openOptions |= MQOO_SET_ALL_CONTEXT;
		putOptions |= MQPMO_SET_ALL_CONTEXT;
	}

	Queue q(qmgr(), qName);
	q.open(openOptions);

	std::string d = input->getValue<std::string>("data");
	message->setFormat("MQSTR");
	message->buffer().resize(d.length());
	memcpy(message->buffer().data(), d.c_str(), d.length());

	q.put(*message, putOptions);
}

} } // Namespace MQ::Web
