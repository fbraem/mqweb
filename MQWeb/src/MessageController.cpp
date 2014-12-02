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

#include "Poco/DateTimeFormatter.h"
#include "Poco/Logger.h"
#include "Poco/HexBinaryEncoder.h"

#include "MQ/Web/MessageController.h"
#include "MQ/Web/MQMapper.h"
#include "MQ/Web/JSONView.h"
#include "MQ/MQException.h"
#include "MQ/Message.h"
#include "MQ/QueueManager.h"
#include "MQ/Queue.h"
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


#define RCSTR(x) (x, #x)

DisplayMap MessageController::_reportCodes = DisplayMapInitializer
	RCSTR(MQRO_EXCEPTION                 )
	RCSTR(MQRO_EXCEPTION_WITH_DATA       )
	RCSTR(MQRO_EXCEPTION_WITH_FULL_DATA  )
	RCSTR(MQRO_EXPIRATION                )
	RCSTR(MQRO_EXPIRATION_WITH_DATA      )
	RCSTR(MQRO_EXPIRATION_WITH_FULL_DATA )
	RCSTR(MQRO_COA                       )
	RCSTR(MQRO_COA_WITH_DATA             )
	RCSTR(MQRO_COA_WITH_FULL_DATA        )
	RCSTR(MQRO_COD                       )
	RCSTR(MQRO_COD_WITH_DATA             )
	RCSTR(MQRO_COD_WITH_FULL_DATA        )
	RCSTR(MQRO_PAN                       )
	RCSTR(MQRO_NAN                       )
	RCSTR(MQRO_ACTIVITY                  )
	RCSTR(MQRO_NEW_MSG_ID                )
	RCSTR(MQRO_PASS_MSG_ID               )
	RCSTR(MQRO_COPY_MSG_ID_TO_CORREL_ID  )
	RCSTR(MQRO_PASS_CORREL_ID            )
	RCSTR(MQRO_DEAD_LETTER_Q             )
	RCSTR(MQRO_DISCARD_MSG               )
	RCSTR(MQRO_PASS_DISCARD_AND_EXPIRY   )
	RCSTR(MQRO_NONE                      )
;

DisplayMap MessageController::_messageTypeCodes = DisplayMapInitializer
	RCSTR(MQMT_REQUEST)
	RCSTR(MQMT_REPLY)
	RCSTR(MQMT_DATAGRAM)
	RCSTR(MQMT_REPORT)
	RCSTR(MQMT_MQE_FIELDS_FROM_MQE)
	RCSTR(MQMT_MQE_FIELDS)
;


DisplayMap MessageController::_feedbackCodes = DisplayMapInitializer
	RCSTR(MQFB_NONE                   )
	RCSTR(MQFB_QUIT                   )
	RCSTR(MQFB_EXPIRATION             )
	RCSTR(MQFB_COA                    )
	RCSTR(MQFB_COD                    )
	RCSTR(MQFB_CHANNEL_COMPLETED      )
	RCSTR(MQFB_CHANNEL_FAIL_RETRY     )
	RCSTR(MQFB_CHANNEL_FAIL           )
	RCSTR(MQFB_APPL_CANNOT_BE_STARTED )
	RCSTR(MQFB_TM_ERROR               )
	RCSTR(MQFB_APPL_TYPE_ERROR        )
	RCSTR(MQFB_STOPPED_BY_MSG_EXIT    )
	RCSTR(MQFB_ACTIVITY               )
	RCSTR(MQFB_XMIT_Q_MSG_ERROR       )
	RCSTR(MQFB_PAN                    )
	RCSTR(MQFB_NAN                    )
	RCSTR(MQFB_STOPPED_BY_CHAD_EXIT   )
	RCSTR(MQFB_STOPPED_BY_PUBSUB_EXIT )
	RCSTR(MQFB_NOT_A_REPOSITORY_MSG   )
	RCSTR(MQFB_BIND_OPEN_CLUSRCVR_DEL )
	RCSTR(MQFB_MAX_ACTIVITIES         )
	RCSTR(MQFB_NOT_FORWARDED          )
	RCSTR(MQFB_NOT_DELIVERED          )
	RCSTR(MQFB_UNSUPPORTED_FORWARDING )
	RCSTR(MQFB_UNSUPPORTED_DELIVERY   )
	RCSTR(MQFB_DATA_LENGTH_ZERO       )
	RCSTR(MQFB_DATA_LENGTH_NEGATIVE   )
	RCSTR(MQFB_DATA_LENGTH_TOO_BIG    )
	RCSTR(MQFB_BUFFER_OVERFLOW        )
	RCSTR(MQFB_LENGTH_OFF_BY_ONE      )
	RCSTR(MQFB_IIH_ERROR              )
	RCSTR(MQFB_NOT_AUTHORIZED_FOR_IMS )
	RCSTR(MQFB_IMS_ERROR              )
	RCSTR(MQFB_IMS_FIRST              )
	RCSTR(MQFB_IMS_LAST               )
	RCSTR(MQFB_CICS_INTERNAL_ERROR    )
	RCSTR(MQFB_CICS_NOT_AUTHORIZED    )
	RCSTR(MQFB_CICS_BRIDGE_FAILURE    )
	RCSTR(MQFB_CICS_CORREL_ID_ERROR   )
	RCSTR(MQFB_CICS_CCSID_ERROR       )
	RCSTR(MQFB_CICS_ENCODING_ERROR    )
	RCSTR(MQFB_CICS_CIH_ERROR         )
	RCSTR(MQFB_CICS_UOW_ERROR         )
	RCSTR(MQFB_CICS_COMMAREA_ERROR    )
	RCSTR(MQFB_CICS_APPL_NOT_STARTED  )
	RCSTR(MQFB_CICS_APPL_ABENDED      )
	RCSTR(MQFB_CICS_DLQ_ERROR         )
	RCSTR(MQFB_CICS_UOW_BACKED_OUT    )
	RCSTR(MQFB_PUBLICATIONS_ON_REQUEST)
	RCSTR(MQFB_SUBSCRIBER_IS_PUBLISHER)
	RCSTR(MQFB_MSG_SCOPE_MISMATCH     )
	RCSTR(MQFB_SELECTOR_MISMATCH      )
	RCSTR(MQFB_NOT_A_GROUPUR_MSG      )
	RCSTR(MQRC_PUT_INHIBITED          )
	RCSTR(MQRC_Q_FULL                 )
	RCSTR(MQRC_NOT_AUTHORIZED         )
	RCSTR(MQRC_Q_SPACE_NOT_AVAILABLE  )
	RCSTR(MQRC_PERSISTENT_NOT_ALLOWED )
	RCSTR(MQRC_MSG_TOO_BIG_FOR_Q_MGR  )
	RCSTR(MQRC_MSG_TOO_BIG_FOR_Q      )
;


MessageController::MessageController() : MQController()
{
}


MessageController::~MessageController()
{
}

/**
 * URL: message/browse/<qmgrName>/<queueName>?limit=n&teaser=n
 *      message/browse/<qmgrName>/<queueName>/<msgId>
 */
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
	mqwebData().set("queue", queueName);

	int limit = -1;
	int teaser = 0;

	std::string messageId;
	if ( parameters.size() > 2 )
	{
		limit = 1;
		messageId = parameters[2];
		mqwebData().set("messageId", messageId);
	}
	else
	{
		std::string limitField = form().get("limit", "");
		if ( ! limitField.empty() )
		{
			Poco::NumberParser::tryParse(limitField, limit);
		}

		std::string teaserField = form().get("teaser", "");
		if ( ! teaserField.empty() )
		{
			Poco::NumberParser::tryParse(teaserField, teaser);
		}
	}

	Poco::JSON::Array::Ptr jsonMessages = new Poco::JSON::Array();

	Queue q(*qmgr(), queueName);
	q.open(MQOO_BROWSE);

	int count = 0;
	while(limit == -1 || count < limit)
	{
		Message msg(teaser);
		if ( ! messageId.empty() )
		{
			msg.messageId()->fromHex(messageId);
		}

		try
		{
			q.get(msg, MQGMO_BROWSE_NEXT + MQGMO_ACCEPT_TRUNCATED_MSG, 0);
		}
		catch(MQException mqe)
		{
			if ( mqe.reason() == MQRC_NO_MSG_AVAILABLE )
			{
				if (! messageId.empty()) throw;
				break;
			}
			else if ( mqe.reason() != MQRC_TRUNCATED_MSG_ACCEPTED )
			{
				throw;
			}
		}

		count++;
		Poco::JSON::Object::Ptr jsonMessage = new Poco::JSON::Object();
		mapMessageToJSON(msg, *jsonMessage);

		std::string data;
		std::string format(msg.getFormat());
		if ( teaser > 0 && msg.getFormat().compare(MQFMT_STRING) == 0 )
		{
			if ( msg.dataLength() < msg.buffer().size() )
			{
				msg.buffer().resize(msg.dataLength());
			}
			data = msg.buffer().toString();
			if ( teaser < msg.dataLength() )
			{
				data += " ...";
			}
			jsonMessage->set("data", data);
		}

		jsonMessages->add(jsonMessage);
	}

	set("messages", jsonMessages);
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
	mqwebData().set("queue", queueName);

	std::string messageId = parameters[2];
	mqwebData().set("messageId", messageId);

	Message message;

	try
	{
		message.messageId()->fromHex(messageId);
	}
	catch(Poco::DataFormatException&)
	{
		//An invalid message id is passed (No valid HEX character)
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "An invalid message id is passed (No valid HEX character)");
		return;
	}

	Queue q(*qmgr(), queueName);
	q.open(MQOO_BROWSE);

	try
	{
		q.get(message, MQGMO_BROWSE_FIRST);
		message.buffer().resize(message.dataLength());
	}
	catch(MQException& mqe)
	{
		if ( mqe.reason()   == MQRC_TRUNCATED_MSG_FAILED
			|| mqe.reason() == MQRC_TRUNCATED )
		{
			//message.buffer().resize(message.dataLength());
			MQLONG size = message.dataLength();
			if ( size > 1024 * 16) size = 1024 * 16;
			message.buffer().resize(size);
			message.clear();
			message.messageId()->fromHex(messageId);

			try
			{
				q.get(message, MQGMO_BROWSE_FIRST);
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

	std::ostringstream oss;
	Poco::HexBinaryEncoder hexEncoder(oss);
	hexEncoder.rdbuf()->setLineLength(0);
	hexEncoder.rdbuf()->setUppercase(true);
	hexEncoder.write((const char*) message.buffer().data(), message.buffer().size());

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
		char ebcdic = EBCDIC_translate_ASCII[message.buffer().chr(i)];

		oss << (isprint((unsigned char) ebcdic) ? ebcdic : '.');
		if ( (i + 1) % 16 == 0 )
		{
			jsonMessageDump = jsonDump->getObject(row++);
			if ( !jsonMessageDump.isNull() )
			{
				//jsonMessageDump->set("ebcdic", htmlize(oss.str()));
				jsonMessageDump->set("ebcdic", oss.str());
				oss.str("");
			}
		}
	}
	jsonMessageDump = jsonDump->getObject(row);
	if ( !jsonMessageDump.isNull() )
	{
		//jsonMessageDump->set("ebcdic", htmlize(oss.str()));
		jsonMessageDump->set("ebcdic", oss.str());
		oss.str("");
	}

	//ASCII
	oss.str("");
	row = 0;
	for(int i = 0; i < message.buffer().size(); ++i)
	{
		oss << (isprint(message.buffer().chr(i)) ? (char) message.buffer().chr(i) : '.');
		if ( (i + 1) % 16 == 0 )
		{
			jsonMessageDump = jsonDump->getObject(row++);
			if ( !jsonMessageDump.isNull() )
			{
				//jsonMessageDump->set("ascii", htmlize(oss.str()));
				jsonMessageDump->set("ascii", oss.str());
				oss.str("");
			}
		}
	}
	jsonMessageDump = jsonDump->getObject(row);
	if ( !jsonMessageDump.isNull() )
	{
		//jsonMessageDump->set("ascii", htmlize(oss.str()));
		jsonMessageDump->set("ascii", oss.str());
		oss.str("");
	}

	mapMessageToJSON(message, *jsonMessage);

	set("message", jsonMessage);
}


/**
 * URL: message/event/<qmgrName>/<queueName>[/<msgId>]
 *
 * List all event messages (when <msgId> is omitted) or
 * browses one event message. A JSON and HTML response is
 * supported.
 */
void MessageController::event()
{
	std::vector<std::string> parameters = getParameters();

	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string queueName = parameters[1];
	Queue q(*qmgr().get(), queueName);
	q.open(MQOO_INQUIRE | MQOO_BROWSE | MQBND_BIND_ON_OPEN);

	std::vector<int> intSelectors;
	intSelectors.push_back(MQIA_CURRENT_Q_DEPTH);
	std::map<int, int> charSelectors;
	std::map<int, int> intResult;
	std::map<int, std::string> charResult;
	q.inquire(intSelectors, charSelectors, intResult, charResult);

	MQLONG curdepth = intResult[MQIA_CURRENT_Q_DEPTH];
	Poco::JSON::Object::Ptr jsonQueue = new Poco::JSON::Object();
	set("queue", jsonQueue);
	jsonQueue->set("name", q.name());
	jsonQueue->set("curdepth", curdepth);

	int limit = -1; // -1 -> no limit
	std::string messageId;
	if ( parameters.size() > 2 )
	{
		limit = 1;
		messageId = parameters[2];
		mqwebData().set("messageId", messageId);
	}
	else
	{
		if ( form().has("limit") )
		{
			Poco::NumberParser::tryParse(form().get("limit"), limit);
		}
	}

	Poco::JSON::Array::Ptr jsonEvents = new Poco::JSON::Array();
	set("events", jsonEvents);

	int count = 0;
	while(limit == -1 || count < limit)
	{
		PCF message(qmgr()->zos());
		if ( !messageId.empty() )
		{
			try
			{
				message.messageId()->fromHex(messageId);
			}
			catch(Poco::DataFormatException&)
			{
				//An invalid message id is passed (No valid HEX character)
				setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "An invalid message id is passed (No valid HEX character)");
				return;
			}
		}

		message.buffer().resize(DEFAULT_EVENT_MESSAGE_SIZE, false);
		try
		{
			q.get(message, MQGMO_BROWSE_NEXT | MQGMO_CONVERT);
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
				message.buffer().resize(message.dataLength(), false);
				message.clear();
				if ( !messageId.empty() )
				{
					message.messageId()->fromHex(messageId);
				}
				q.get(message, MQGMO_BROWSE_NEXT | MQGMO_CONVERT);
			}
			else
			{
				throw;
			}
		}

		message.buffer().resize(message.dataLength());
		message.init();

		Poco::JSON::Object::Ptr jsonEventMessage = new Poco::JSON::Object();
		jsonEvents->add(jsonEventMessage);

		Poco::JSON::Object::Ptr jsonMessage = new Poco::JSON::Object();
		jsonEventMessage->set("message", jsonMessage);
		mapMessageToJSON(message, *jsonMessage);

		Poco::JSON::Object::Ptr jsonEvent = new Poco::JSON::Object();
		jsonEventMessage->set("event", jsonEvent);

		Poco::JSON::Object::Ptr jsonReason = new Poco::JSON::Object();
		jsonEvent->set("reason", jsonReason);
		jsonReason->set("code", message.getReasonCode());
		std::string reasonCodeStr = MQMapper::getReasonString(message.getReasonCode());
		jsonReason->set("desc", reasonCodeStr);

		if ( message.hasParameter(MQIACF_OBJECT_TYPE) )
		{
			Poco::SharedPtr<Dictionary> dictionary;
			switch(message.getParameterNum(MQIACF_OBJECT_TYPE))
			{
			case MQOT_Q_MGR: dictionary = MQMapper::dictionary("QueueManager"); break;
			case MQOT_CHANNEL:  dictionary = MQMapper::dictionary("Channel"); break;
			case MQOT_NAMELIST: dictionary = MQMapper::dictionary("Namelist"); break;
			case MQOT_PROCESS: dictionary = MQMapper::dictionary("Process"); break;
			case MQOT_Q: dictionary = MQMapper::dictionary("Queue"); break;
			case MQOT_LISTENER: dictionary = MQMapper::dictionary("Listener"); break;
			default:
				Poco::Logger::get("mq.web").warning("No dictionary set for event. ObjectType $0", message.getParameterNum(MQIACF_OBJECT_TYPE));
			}
			if ( !dictionary.isNull() )
			{
				dictionary->mapToJSON(message, jsonEvent, false);
			}
		}

		Poco::SharedPtr<Dictionary> dictionary = MQMapper::dictionary("Event");
		poco_assert_dbg(! dictionary.isNull());
		dictionary->mapToJSON(message, jsonEvent, false);

		count++;
	}
}


void MessageController::mapMessageToJSON(const Message& message, Poco::JSON::Object& obj)
{
	DisplayMap::const_iterator it = _reportCodes.find(message.getReport());
	obj.set("Report", it == _reportCodes.end() ? "" : it->second);

	it = _messageTypeCodes.find(message.getMsgType());
	obj.set("MsgType", it == _messageTypeCodes.end() ? "" : it->second);

	obj.set("Expiry", message.getExpiry());

	it = _feedbackCodes.find(message.getFeedback());
	if ( it == _feedbackCodes.end() )
	{
		obj.set("Feedback", message.getFeedback());
	}
	else
	{
		obj.set("Feedback", it->second);
	}

	obj.set("Encoding", message.getEncoding());
	obj.set("CodedCharSetId", message.getCodedCharSetId());
	obj.set("Format", message.getFormat());
	obj.set("Priority", message.getPriority());
	obj.set("Persistence", message.getPersistence());
	obj.set("MsgId", message.messageId()->toHex());
	obj.set("CorrelId", message.correlationId()->toHex());
	obj.set("BackoutCount", message.backoutCount());
	obj.set("ReplyToQ", message.getReplyToQueue());
	obj.set("ReplyToQmgr", message.getReplyToQMgr());
	obj.set("UserIdentifier", message.getUser());
	obj.set("AccountingToken", message.accountingToken()->toHex());
	obj.set("ApplIdentityData", message.getApplIdentityData());

	const DisplayMap& applTypes = MQMapper::getDisplayMap("QueueStatus", MQIA_APPL_TYPE);
	it = applTypes.find(message.getPutApplType());
	obj.set("PutApplType", it == applTypes.end() ? "" : it->second);

	obj.set("PutApplName", message.getPutApplName());
	obj.set("PutDate", Poco::DateTimeFormatter::format(message.getPutDate(), "%d-%m-%Y %H:%M:%S"));
	obj.set("ApplOriginData", message.getApplOriginData());
	obj.set("GroupId", message.groupId()->toHex());
	obj.set("MsgSeqNumber", message.getMsgSeqNumber());
	obj.set("Offset", message.getOffset());
	obj.set("MsgFlags", message.getMsgFlags());
	obj.set("OriginalLength", message.getOriginalLength());
	obj.set("Length", message.dataLength());
}

} } // Namespace MQ::Web
