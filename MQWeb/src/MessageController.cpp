/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they
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
#include <iostream>
#include <sstream>
#include <iomanip>

#include <Poco/DateTimeFormatter.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>
#include <Poco/Logger.h>
#include <Poco/HexBinaryEncoder.h>

#include <MQ/Web/MessageController.h>
#include <MQ/Web/MQMapper.h>
#include <MQ/MQException.h>
#include <MQ/Message.h>
#include <MQ/QueueManager.h>
#include <MQ/Queue.h>
#include <MQ/Buffer.h>

/*
static  unsigned char
	EBCDIC_translate_ASCII[256] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		0x2E, 0x2E, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x2E, 0x3F,
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
*/
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

namespace MQ {
namespace Web {

MessageController::MessageController() : MQController()
{
}


MessageController::~MessageController()
{
}


void MessageController::list()
{
	if ( !isPost() )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter is queuename
	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string queueName = parameters[1];
	Poco::JSON::Object::Ptr jsonQueue = new Poco::JSON::Object();
	jsonQueue->set("name", queueName);
	set("queue", jsonQueue);

	Poco::Net::HTMLForm form(request(), request().stream());

	std::string limitField = form.get("limit", "");
	int limit = -1;
	if ( ! limitField.empty() )
	{
		Poco::NumberParser::tryParse(limitField, limit);
	}

	std::string teaserField = form.get("teaser", "");
	int teaser = 0;
	if ( ! teaserField.empty() )
	{
		Poco::NumberParser::tryParse(teaserField, teaser);
	}

	Poco::JSON::Array::Ptr jsonMessages = new Poco::JSON::Array();

	Queue q(qmgr(), queueName);
	q.open(MQOO_BROWSE);

	int count = 0;
	while(1)
	{
		Message msg(teaser);
		try
		{
			q.get(msg, MQGMO_BROWSE_NEXT + MQGMO_ACCEPT_TRUNCATED_MSG, 0);
		}
		catch(MQException mqe)
		{
			if ( mqe.reason() == MQRC_NO_MSG_AVAILABLE )
			{
				break;
			}
			if ( mqe.reason() != MQRC_TRUNCATED_MSG_ACCEPTED )
			{
				throw;
			}
		}

		count++;
		Poco::JSON::Object::Ptr jsonMessage = new Poco::JSON::Object();

		BufferPtr id = msg.getMessageId();
		std::stringstream hexId;
		for(int i = 0; i < id->size(); ++i)
		{
			hexId << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int) (*id)[i];
		}
		jsonMessage->set("id", hexId.str());

		jsonMessage->set("putDate", Poco::DateTimeFormatter::format(msg.getPutDate(), "%d-%m-%Y %H:%M:%S"));
		jsonMessage->set("user", msg.getUser());
		jsonMessage->set("putApplication", msg.getPutApplication());
		jsonMessage->set("format", msg.getFormat());
		jsonMessage->set("length", msg.dataLength());
		jsonMessage->set("encoding", msg.getEncoding());
		jsonMessage->set("ccsid", msg.getCodedCharSetId());

		std::string data;
		if ( teaser > 0 && msg.getFormat().compare(MQFMT_STRING) == 0 )
		{
			if ( msg.dataLength() < msg.buffer().size() )
			{
				msg.buffer().resize(msg.dataLength());
			}
			data = std::string(msg.buffer().begin(), msg.buffer().end());
			if ( teaser < msg.dataLength() )
			{
				data += " ...";
			}
			jsonMessage->set("data", htmlize(data));
		}

		jsonMessages->add(jsonMessage);
	}

	set("messages", jsonMessages);
	render("messageList.tpl");
}


void MessageController::view()
{
	std::vector<std::string> parameters = getParameters();

	if ( parameters.size() < 3 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string queueName = parameters[1];
	std::string messageId = parameters[2];

	Buffer id(MQ_MSG_ID_LENGTH);
	for(int i = 0; i < MQ_MSG_ID_LENGTH; i++)
	{
		std::stringstream ss;
		int val;
		ss << std::setbase(16) << "0x" << messageId.substr(i * 2, 2);
		ss >> val;
		id[i] = (unsigned char) val;
	}

	Message message;
	message.setMessageId(id);

	Queue q(qmgr(), queueName);
	q.open(MQOO_BROWSE);

	try
	{
		//message.setCodedCharSetId(1208);
		q.get(message, MQGMO_BROWSE_FIRST /*| MQGMO_CONVERT*/);
	}
	catch(MQException& mqe)
	{
		if ( mqe.reason()   == MQRC_TRUNCATED_MSG_FAILED
			|| mqe.reason() == MQRC_TRUNCATED )
		{
			//message.setCodedCharSetId(1208);
			message.buffer().resize(message.dataLength());
			q.get(message, MQGMO_BROWSE_FIRST /*| MQGMO_CONVERT*/);
		}
		else
		{
			throw;
		}
	}
	message.buffer().resize(message.dataLength());

	Poco::Net::HTMLForm form(request());
	std::string type = form.get("type", "detail");

	Poco::JSON::Object::Ptr jsonMessage = new Poco::JSON::Object();

	if ( type.compare("hex") == 0 )
	{
		Poco::JSON::Array::Ptr jsonDump = new Poco::JSON::Array();
		jsonMessage->set("dump", jsonDump);

		std::ostringstream oss;
		Poco::HexBinaryEncoder hexEncoder(oss);
		hexEncoder.rdbuf()->setLineLength(0);
		hexEncoder.rdbuf()->setUppercase(true);
		hexEncoder.write((const char*) &message.buffer()[0], message.buffer().size());

		std::string fullHex = oss.str();
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

		//EBCDIC
		oss.str("");
		int row = 0;
		for(int i = 0; i < message.buffer().size(); ++i)
		{
			char ebcdic = EBCDIC_translate_ASCII[message.buffer()[i]];

			oss << (isprint((unsigned char) ebcdic) ? ebcdic : '.');
			if ( (i + 1) % 16 == 0 )
			{
				jsonMessageDump = jsonDump->getObject(row++);
				if ( !jsonMessageDump.isNull() )
				{
					jsonMessageDump->set("ebcdic", htmlize(oss.str()));
					oss.str("");
				}
			}
		}
		jsonMessageDump = jsonDump->getObject(row);
		if ( !jsonMessageDump.isNull() )
		{
			jsonMessageDump->set("ebcdic", htmlize(oss.str()));
			oss.str("");
		}

		//ASCII
		oss.str("");
		row = 0;
		for(int i = 0; i < message.buffer().size(); ++i)
		{
			oss << (isprint(message.buffer()[i]) ? (char) message.buffer()[i] : '.');
			if ( (i + 1) % 16 == 0 )
			{
				jsonMessageDump = jsonDump->getObject(row++);
				if ( !jsonMessageDump.isNull() )
				{
					jsonMessageDump->set("ascii", htmlize(oss.str()));
					oss.str("");
				}
			}
		}
		jsonMessageDump = jsonDump->getObject(row);
		if ( !jsonMessageDump.isNull() )
		{
			jsonMessageDump->set("ascii", htmlize(oss.str()));
			oss.str("");
		}

		jsonMessage->set("id", messageId);
	}
	else if ( type.compare("detail") == 0 )
	{
		jsonMessage->set("id", messageId);
	}

	set("message", jsonMessage);
	render("message.tpl");
}


void MessageController::event()
{
	std::vector<std::string> parameters = getParameters();

	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	Poco::JSON::Array::Ptr jsonEvents = new Poco::JSON::Array();
	set("events", jsonEvents);

	std::string queueName = parameters[1];
	Queue q(qmgr(), queueName);
	q.open(MQOO_BROWSE);

	std::string messageId;
	if ( parameters.size() > 2 )
	{
		messageId = parameters[2];

		Buffer id(MQ_MSG_ID_LENGTH);
		for(int i = 0; i < MQ_MSG_ID_LENGTH; i++)
		{
			std::stringstream ss;
			int val;
			ss << std::setbase(16) << "0x" << messageId.substr(i * 2, 2);
			ss >> val;
			id[i] = (unsigned char) val;
		}

		PCF message(qmgr()->zos());
		message.setMessageId(id);

		try
		{
			q.get(message, MQGMO_BROWSE_FIRST | MQGMO_CONVERT);
		}
		catch(MQException& mqe)
		{
			if ( mqe.reason()   == MQRC_TRUNCATED_MSG_FAILED
				|| mqe.reason() == MQRC_TRUNCATED )
			{
				message.buffer().resize(message.dataLength(), false);
				message.clear();
				message.setMessageId(id);
				q.get(message, MQGMO_BROWSE_FIRST | MQGMO_CONVERT);
			}
			else
			{
				throw;
			}
		}
		message.buffer().resize(message.dataLength());
		message.init();

		Poco::JSON::Object::Ptr jsonReason = new Poco::JSON::Object();
		set("reason", jsonReason);
		jsonReason->set("code", message.getReasonCode());
		std::string reasonCodeStr = MQMapper::getReasonString(message.getReasonCode());
		jsonReason->set("desc", reasonCodeStr);

		Poco::JSON::Object::Ptr jsonEvent = new Poco::JSON::Object();
		set("event", jsonEvent);
		jsonEvent->set("putDate", Poco::DateTimeFormatter::format(message.getPutDate(), "%d-%m-%Y %H:%M:%S"));
		MQMapper::mapToJSON(message, jsonEvent);

		std::string templateName;
		if ( message.getReasonCode() == MQRC_NOT_AUTHORIZED )
		{
			switch(message.getParameterNum(MQIACF_REASON_QUALIFIER))
			{
			case MQRQ_CONN_NOT_AUTHORIZED:
			case MQRQ_SYS_CONN_NOT_AUTHORIZED:
				templateName = Poco::format("events/%s-1.tpl", reasonCodeStr);
				break;
			case MQRQ_OPEN_NOT_AUTHORIZED:
				templateName = Poco::format("events/%s-2.tpl", reasonCodeStr);
				break;
			case MQRQ_CLOSE_NOT_AUTHORIZED:
				templateName = Poco::format("events/%s-3.tpl", reasonCodeStr);
				break;
			case MQRQ_CMD_NOT_AUTHORIZED:
				templateName = Poco::format("events/%s-4.tpl", reasonCodeStr);
				break;
			case MQRQ_SUB_NOT_AUTHORIZED:
				templateName = Poco::format("events/%s-5.tpl", reasonCodeStr);
				break;
			case MQRQ_SUB_DEST_NOT_AUTHORIZED:
				templateName = Poco::format("events/%s-6.tpl", reasonCodeStr);
				break;
			}
		}
		else
		{
			templateName = Poco::format("events/%s.tpl", reasonCodeStr);
		}
		render(templateName);

		return;
	}
	else // Get all event messages
	{
		int count = 0;
		while(1)
		{
			PCF message(qmgr()->zos());
			message.buffer().resize(1000, false);
			try
			{
				q.get(message, MQGMO_BROWSE_NEXT | MQGMO_CONVERT);
			}
			catch(MQException& mqe)
			{
				if ( mqe.reason() == MQRC_NO_MSG_AVAILABLE )
				{
					break;
				}

				if ( mqe.reason()   == MQRC_TRUNCATED_MSG_FAILED
					|| mqe.reason() == MQRC_TRUNCATED )
				{
					message.buffer().resize(message.dataLength(), false);
					message.clear();
					q.get(message, MQGMO_BROWSE_NEXT | MQGMO_CONVERT);
				}
				else
				{
					throw;
				}
			}
			message.buffer().resize(message.dataLength());
			message.init();

			Poco::JSON::Object::Ptr jsonEvent = new Poco::JSON::Object();
			jsonEvents->add(jsonEvent);

			jsonEvent->set("reason", message.getReasonCode());
			jsonEvent->set("desc", MQMapper::getReasonString(message.getReasonCode()));
			jsonEvent->set("putDate", Poco::DateTimeFormatter::format(message.getPutDate(), "%d-%m-%Y %H:%M:%S"));
			BufferPtr id = message.getMessageId();
			std::stringstream hexId;
			for(int i = 0; i < id->size(); ++i)
			{
				hexId << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int) (*id)[i];
			}
			jsonEvent->set("id", hexId.str());

			Poco::JSON::Object::Ptr jsonEventFields = new Poco::JSON::Object();
			jsonEvent->set("fields", jsonEventFields);
			MQMapper::mapToJSON(message, jsonEventFields);
		}
	}

	render("events.tpl");
}


} } // Namespace MQ::Web
