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
#include "Poco/Timezone.h"

#include "MQ/Web/MessageController.h"
#include "MQ/Web/MQMapper.h"
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


#define RCSTR(x) (x, #x)

Dictionary MessageController::_reportCodes = Dictionary()
	(0, "Report", TextMapInitializer
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
	)
;

Dictionary MessageController::_messageTypeCodes = Dictionary()
	(0, "MsgType", TextMapInitializer
		RCSTR(MQMT_REQUEST)
		RCSTR(MQMT_REPLY)
		RCSTR(MQMT_DATAGRAM)
		RCSTR(MQMT_REPORT)
		RCSTR(MQMT_MQE_FIELDS_FROM_MQE)
		RCSTR(MQMT_MQE_FIELDS)
	)
;

Dictionary MessageController::_feedbackCodes = Dictionary()
	(0, "Feedback", TextMapInitializer
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
	)
;


MessageController::MessageController() : MQController()
{
}


MessageController::~MessageController()
{
}

/**
 * URL: message/browse/<qmgrName>/<queueName>?limit=n&size=n
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
	meta().set("queue", queueName);

	int limit = -1;
	int maxMessageSize = 1024;

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

		std::string sizeField = form().get("size", "");
		if ( ! sizeField.empty() )
		{
			Poco::NumberParser::tryParse(sizeField, maxMessageSize);
		}
	}

	Poco::JSON::Array::Ptr jsonMessages = new Poco::JSON::Array();

	Queue q(*qmgr(), queueName);
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
				if ( msg->getFormat().compare(MQFMT_EVENT ) == 0 ) {
					// ignore size limit for event messages and retry to get it with the
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
					continue; // TODO: or throw ???;
				}
			}
			else
			{
				throw;
			}
		}

		count++;
		Poco::JSON::Object::Ptr jsonMessage = new Poco::JSON::Object();
		mapMessageToJSON(*msg, *jsonMessage);

		if ( msg->getFormat().compare(MQFMT_DEAD_LETTER_HEADER) == 0 )
		{
			if ( msg->buffer().size() >= sizeof(MQDLH) )
			{
				MQDLH* dlh = (MQDLH*) msg->buffer().data();
				Poco::JSON::Object::Ptr jsonDLH = new Poco::JSON::Object();
				jsonMessage->set("dlh", jsonDLH);
				jsonDLH->set("Version", dlh->Version);

				Poco::JSON::Object::Ptr jsonReason = new Poco::JSON::Object();
				jsonDLH->set("Reason", jsonReason);
				jsonReason->set("code", dlh->Reason);
				std::string reasonCodeStr = MQMapper::getReasonString(dlh->Reason);
				jsonReason->set("desc", reasonCodeStr);

				jsonDLH->set("DestQName", Poco::trimRight(std::string(dlh->DestQName, MQ_Q_NAME_LENGTH)));
				jsonDLH->set("DestQMgrName", Poco::trimRight(std::string(dlh->DestQMgrName, MQ_Q_MGR_NAME_LENGTH)));
				jsonDLH->set("Encoding", dlh->Encoding);
				jsonDLH->set("CodedCharSetId", dlh->CodedCharSetId);
				jsonDLH->set("Format", Poco::trimRight(std::string(dlh->Format, MQ_FORMAT_LENGTH)));
				const TextMap& applTypes = MQMapper::getTextMap("QueueStatus", MQIA_APPL_TYPE);
				TextMap::const_iterator it = applTypes.find(dlh->PutApplType);
				jsonDLH->set("PutApplType", it == applTypes.end() ? "" : it->second);
				jsonDLH->set("PutApplName", Poco::trimRight(std::string(dlh->PutApplName, MQ_PUT_APPL_NAME_LENGTH)));

				std::string putDate = Poco::trimRight(std::string(dlh->PutDate, MQ_PUT_DATE_LENGTH));
				std::string putTime = Poco::trimRight(std::string(dlh->PutTime, MQ_PUT_TIME_LENGTH));
				jsonDLH->set("PutDatetime", Poco::DateTimeFormatter::format(Message::createDatetime(putDate, putTime), "%Y-%m-%d %H:%M:%S.%i"));
				jsonDLH->set("PutDate", putDate);
				jsonDLH->set("PutTime", putTime);
			}
		}
		else if ( msg->getFormat().compare(MQFMT_EVENT) == 0 )
		{
			PCF pcfEvent(msg);

			Poco::JSON::Object::Ptr jsonEvent = new Poco::JSON::Object();
			jsonMessage->set("event", jsonEvent);
			Poco::JSON::Object::Ptr jsonReason = new Poco::JSON::Object();
			jsonEvent->set("Reason", jsonReason);
			jsonReason->set("code", pcfEvent.getReasonCode());
			std::string reasonCodeStr = MQMapper::getReasonString(pcfEvent.getReasonCode());
			jsonReason->set("desc", reasonCodeStr);

			if ( pcfEvent.hasParameter(MQIACF_OBJECT_TYPE) )
			{
				Poco::SharedPtr<Dictionary> dictionary;
				switch(pcfEvent.getParameterNum(MQIACF_OBJECT_TYPE))
				{
				case MQOT_Q_MGR: dictionary = MQMapper::dictionary("QueueManager"); break;
				case MQOT_CHANNEL:  dictionary = MQMapper::dictionary("Channel"); break;
				case MQOT_NAMELIST: dictionary = MQMapper::dictionary("Namelist"); break;
				case MQOT_PROCESS: dictionary = MQMapper::dictionary("Process"); break;
				case MQOT_Q: dictionary = MQMapper::dictionary("Queue"); break;
				case MQOT_LISTENER: dictionary = MQMapper::dictionary("Listener"); break;
				default:
					Poco::Logger::get("mq.web").warning("No dictionary set for event. ObjectType $0", pcfEvent.getParameterNum(MQIACF_OBJECT_TYPE));
				}
				if ( !dictionary.isNull() )
				{
					dictionary->mapToJSON(pcfEvent, jsonEvent, false);
				}
			}

			Poco::SharedPtr<Dictionary> dictionary = MQMapper::dictionary("Event");
			poco_assert_dbg(! dictionary.isNull());
			dictionary->mapToJSON(pcfEvent, jsonEvent, false);
		}
		else if ( msg->getFormat().compare(MQFMT_STRING) == 0 )
		{
			if ( msg->dataLength() < msg->buffer().size() )
			{
				msg->buffer().resize(msg->dataLength());
			}
			jsonMessage->set("data", msg->buffer().toString());
		}
		else if ( msg->getFormat().compare(MQFMT_RF_HEADER_2) == 0 )
		{
			MQBYTE* begin =  (MQBYTE*) msg->buffer().data();
			MQRFH2* rfh2 = (MQRFH2*) begin;
			Poco::JSON::Object::Ptr jsonRfh2 = new Poco::JSON::Object();
			jsonMessage->set("rfh2", jsonRfh2);
			jsonRfh2->set("Encoding", rfh2->Encoding);

			Poco::JSON::Array::Ptr jsonNameValues = new Poco::JSON::Array();
			jsonRfh2->set("NameValues", jsonNameValues);

			MQBYTE* data = begin + sizeof(MQRFH2);
			MQBYTE* end = begin + rfh2->StrucLength;

			while(data < end)
			{
				MQLONG len = *(MQLONG*) data;
				data += sizeof(MQLONG);

				if ( len < 0 || data + len > end )
				{
					//TODO: invalid RFH2 ...
				}
				std::string value((const char*) data, len);
				if ( value[0] == '\0' ) value.resize(0);
				Poco::trimRightInPlace(value);
				jsonNameValues->add(value);
				data += len;
			}
		}
		jsonMessages->add(jsonMessage);
	}

	set("data", jsonMessages);
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

	std::string fullHex = message.buffer().toHex();
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
	for(int i = 0; i < message.buffer().size(); ++i)
	{
		unsigned char code = message.buffer().chr(i);
		oss << (char) (message.getCodedCharSetId() == 500 ? EBCDIC_translate_ASCII[code] : code);
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
	mapMessageToJSON(message, *jsonMessage);

	set("data", jsonMessage);
}


void MessageController::mapMessageToJSON(const Message& message, Poco::JSON::Object& obj)
{
	std::string report = _reportCodes.getTextForValue(0, message.getReport());
	obj.set("Report", report);

	std::string msgType = _messageTypeCodes.getTextForValue(0, message.getMsgType());
	if ( msgType.empty() )
	{
		obj.set("MsgType", message.getMsgType());
	}
	else
	{
		obj.set("MsgType", msgType);
	}

	obj.set("Expiry", message.getExpiry());
	obj.set("Feedback", _feedbackCodes.getTextForValue(0, message.getFeedback()));
	obj.set("Encoding", message.getEncoding());
	obj.set("CodedCharSetId", message.getCodedCharSetId());
	obj.set("Format", Poco::trimRight(message.getFormat()));
	obj.set("Priority", message.getPriority());
	obj.set("Persistence", message.getPersistence());
	obj.set("MsgId", message.messageId()->toHex());
	obj.set("CorrelId", message.correlationId()->toHex());
	obj.set("BackoutCount", message.getBackoutCount());
	obj.set("ReplyToQ", message.getReplyToQueue());
	obj.set("ReplyToQmgr", message.getReplyToQMgr());
	obj.set("UserIdentifier", message.getUser());
	obj.set("AccountingToken", message.accountingToken()->toHex());
	obj.set("ApplIdentityData", message.getApplIdentityData());

	const TextMap& applTypes = MQMapper::getTextMap("QueueStatus", MQIA_APPL_TYPE);
	TextMap::const_iterator it = applTypes.find(message.getPutApplType());
	obj.set("PutApplType", it == applTypes.end() ? "" : it->second);

	obj.set("PutApplName", message.getPutApplName());
	obj.set("PutDatetime", Poco::DateTimeFormatter::format(message.getPutDatetime(), "%Y-%m-%d %H:%M:%S.%i"));
	obj.set("PutDate", message.getPutDate());
	obj.set("PutTime", message.getPutTime());
	obj.set("ApplOriginData", message.getApplOriginData());
	obj.set("GroupId", message.groupId()->toHex());
	obj.set("MsgSeqNumber", message.getMsgSeqNumber());
	obj.set("Offset", message.getOffset());
	obj.set("MsgFlags", message.getMsgFlags());
	obj.set("OriginalLength", message.getOriginalLength());
	obj.set("Length", message.dataLength());
}


void MessageController::mapJSONToMessage(const Poco::JSON::Object& obj, Message& message)
{
	// Set the putdate to the current timestamp to avoid an empty putdate when
	// it is not set in the MD JSON object
	Poco::DateTime now;
	message.setPutDatetime(now);

	if ( obj.has("Report") )
	{
		MQLONG reportCode = _reportCodes.getIdForText(0, obj.optValue<std::string>("Report", ""));
		if ( reportCode != -1 )
		{
			message.setReport(reportCode);
		}
	}

	if ( obj.has("Expiry") ) message.setExpiry(obj.optValue<MQLONG>("Expiry", -1));

	if ( obj.has("Feedback") )
	{
		MQLONG feedbackCode = _feedbackCodes.getIdForText(0, obj.optValue<std::string>("Feedback", ""));
		if ( feedbackCode != -1 )
		{
			message.setFeedback(feedbackCode);
		}
	}

	if ( obj.has("Encoding") ) message.setEncoding(obj.optValue<MQLONG>("Encoding", MQENC_NATIVE));
	if ( obj.has("CodedCharSetId") ) message.setCodedCharSetId(obj.optValue<MQLONG>("CodedCharSetId", MQCCSI_Q_MGR));
	if ( obj.has("Format") ) message.setFormat(obj.optValue<std::string>("Format", ""));
	if ( obj.has("Priority") ) message.setPriority(obj.optValue<MQLONG>("Priority", 0));
	if ( obj.has("Persistence") )
	{
		bool persistency = obj.optValue<bool>("Persistence", false);
		message.setPersistence(persistency);
	}
	if ( obj.has("MsgId") )
	{
		std::string hex = obj.optValue<std::string>("MsgId", "");
		message.messageId()->fromHex(hex);
	}
	if ( obj.has("CorrelId") )
	{
		std::string hex = obj.optValue<std::string>("CorrelId", "");
		message.correlationId()->fromHex(hex);
	}
	if ( obj.has("BackoutCount") ) message.setBackoutCount(obj.optValue<MQLONG>("BackoutCount", 0));
	if ( obj.has("ReplyToQ") ) message.setReplyToQueue(obj.optValue<std::string>("ReplyToQ", ""));
	if ( obj.has("ReplyToQMgr") ) message.setReplyToQMgr(obj.optValue<std::string>("ReplyToQMgr", ""));
	if ( obj.has("UserIdentifier") ) message.setUser(obj.optValue<std::string>("UserIdentifier", ""));
	if ( obj.has("AccountToken") )
	{
		std::string hex = obj.optValue<std::string>("AccountToken", "");
		message.accountingToken()->fromHex(hex);
	}
	if ( obj.has("ApplIdentityData") ) message.setApplIdentityData(obj.optValue<std::string>("ApplIdentityData", ""));
	if ( obj.has("PutApplType") )
	{
		Poco::SharedPtr<Dictionary> dictionary = MQMapper::dictionary("QueueStatus");
		MQLONG id = dictionary->getIdForText(MQIA_APPL_TYPE, obj.optValue<std::string>("PutApplType", ""));
		if ( id == -1 ) // User defined?
		{
			message.setPutApplType(obj.optValue<MQLONG>("PutApplType", MQAT_DEFAULT));
		}
		else
		{
			message.setPutApplType(obj.optValue<MQLONG>("PutApplType", id));
		}
	}
	if ( obj.has("PutApplName") ) message.setPutApplName(obj.optValue<std::string>("PutApplName", ""));
	if ( obj.has("PutDate") ) message.setPutDate(obj.optValue<std::string>("PutDate", ""));
	if ( obj.has("PutTime") ) message.setPutDate(obj.optValue<std::string>("PutTime", ""));
	if ( obj.has("PutDatetime") )
	{
		int tzd;
		try 
		{
			Poco::DateTime dateTime = Poco::DateTimeParser::parse("%Y-%m-%d %H:%M:%S.%i", obj.optValue<std::string>("PutDatetime", ""), tzd);
			message.setPutDatetime(dateTime, tzd);
		}
		catch(Poco::SyntaxException&)
		{
		}
	}
	if ( obj.has("ApplOriginData") ) message.setApplOriginData(obj.optValue<std::string>("ApplOriginData", ""));

	// MQMQ_VERSION_2 fields
	if ( obj.has("GroupId") )
	{
		std::string hex = obj.optValue<std::string>("GroupId", "");
		message.groupId()->fromHex(hex);
	}
	if ( obj.has("MsgFlags") ) message.setMsgFlags(obj.optValue<MQLONG>("MsgFlags", 0));
	if ( obj.has("MsgSeqNumber") ) message.setMsgSeqNumber(obj.optValue<MQLONG>("MsgSeqNumber", 0));
	if ( obj.has("MsgType") )
	{
		MQLONG msgTypeCode = _messageTypeCodes.getIdForText(0, obj.optValue<std::string>("MsgType", ""));
		if ( msgTypeCode != -1 )
		{
			message.setMsgType(msgTypeCode);
		}
	}
	if ( obj.has("Offset") ) message.setOffset(obj.optValue<MQLONG>("Offset", 0));
	if ( obj.has("OriginalLength") ) message.setOriginalLength(obj.optValue<MQLONG>("OriginalLength", 0));
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
		topic = new Topic(*qmgr().get(), topicName, parameters[2]);
	}
	else
	{
		topic = new Topic(*qmgr().get(), topicName);
	}

	std::string topicString;
	if ( parameters.size() > 2 )
	{
		topicString = parameters[2];
	}

	MQ::Message message;

	Poco::JSON::Object::Ptr input = data().getObject("input");
	if ( input.isNull() )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "JSON object missing");
		return;
	}

	MQLONG openOptions = MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING | MQBND_BIND_ON_OPEN;
	MQLONG publishOptions = 0;
	Poco::JSON::Object::Ptr md = input->getObject("MD");
	if ( ! md.isNull() )
	{
		mapJSONToMessage(*md, message);
		openOptions |= MQOO_SET_ALL_CONTEXT;
		publishOptions |= MQPMO_SET_ALL_CONTEXT;
	}
	topic->open(openOptions);

	std::string d = input->getValue<std::string>("data");
	message.setFormat("MQSTR");
	message.buffer().resize(d.length());
	memcpy(message.buffer().data(), d.c_str(), d.length());

	topic->publish(message, publishOptions);
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

	MQ::Message message;

	Poco::JSON::Object::Ptr input = data().getObject("input");
	if ( input.isNull() )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "JSON object missing");
		return;
	}

	MQLONG openOptions = MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING | MQBND_BIND_ON_OPEN;
	MQLONG putOptions = 0;
	Poco::JSON::Object::Ptr md = input->getObject("MD");
	if ( ! md.isNull() )
	{
		mapJSONToMessage(*md, message);
		openOptions |= MQOO_SET_ALL_CONTEXT;
		putOptions |= MQPMO_SET_ALL_CONTEXT;
	}

	Queue q(*qmgr().get(), qName);
	q.open(openOptions);

	std::string d = input->getValue<std::string>("data");
	message.setFormat("MQSTR");
	message.buffer().resize(d.length());
	memcpy(message.buffer().data(), d.c_str(), d.length());

	q.put(message, putOptions);
}

} } // Namespace MQ::Web
