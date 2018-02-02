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

#include "Poco/Logger.h"

#include "MQ/Web/JSONMessage.h"
#include "MQ/Web/PCFCommand.h"

namespace MQ {
namespace Web {

#define RCSTR(x) (x, #x)

Dictionary JSONMessage::_reportCodes = Dictionary()
	(0, "Report", TextMapInitializer
		RCSTR(MQRO_EXCEPTION)
		RCSTR(MQRO_EXCEPTION_WITH_DATA)
		RCSTR(MQRO_EXCEPTION_WITH_FULL_DATA)
		RCSTR(MQRO_EXPIRATION)
		RCSTR(MQRO_EXPIRATION_WITH_DATA)
		RCSTR(MQRO_EXPIRATION_WITH_FULL_DATA)
		RCSTR(MQRO_COA)
		RCSTR(MQRO_COA_WITH_DATA)
		RCSTR(MQRO_COA_WITH_FULL_DATA)
		RCSTR(MQRO_COD)
		RCSTR(MQRO_COD_WITH_DATA)
		RCSTR(MQRO_COD_WITH_FULL_DATA)
		RCSTR(MQRO_PAN)
		RCSTR(MQRO_NAN)
		RCSTR(MQRO_ACTIVITY)
		RCSTR(MQRO_NEW_MSG_ID)
		RCSTR(MQRO_PASS_MSG_ID)
		RCSTR(MQRO_COPY_MSG_ID_TO_CORREL_ID)
		RCSTR(MQRO_PASS_CORREL_ID)
		RCSTR(MQRO_DEAD_LETTER_Q)
		RCSTR(MQRO_DISCARD_MSG)
		RCSTR(MQRO_PASS_DISCARD_AND_EXPIRY)
		RCSTR(MQRO_NONE)
	)
;

Dictionary JSONMessage::_messageTypeCodes = Dictionary()
	(0, "MsgType", TextMapInitializer
		RCSTR(MQMT_REQUEST)
		RCSTR(MQMT_REPLY)
		RCSTR(MQMT_DATAGRAM)
		RCSTR(MQMT_REPORT)
		RCSTR(MQMT_MQE_FIELDS_FROM_MQE)
		RCSTR(MQMT_MQE_FIELDS)
	)
;

Dictionary JSONMessage::_feedbackCodes = Dictionary()
	(0, "Feedback", TextMapInitializer
		RCSTR(MQFB_NONE)
		RCSTR(MQFB_QUIT)
		RCSTR(MQFB_EXPIRATION)
		RCSTR(MQFB_COA)
		RCSTR(MQFB_COD)
		RCSTR(MQFB_CHANNEL_COMPLETED)
		RCSTR(MQFB_CHANNEL_FAIL_RETRY)
		RCSTR(MQFB_CHANNEL_FAIL)
		RCSTR(MQFB_APPL_CANNOT_BE_STARTED)
		RCSTR(MQFB_TM_ERROR)
		RCSTR(MQFB_APPL_TYPE_ERROR)
		RCSTR(MQFB_STOPPED_BY_MSG_EXIT)
		RCSTR(MQFB_ACTIVITY)
		RCSTR(MQFB_XMIT_Q_MSG_ERROR)
		RCSTR(MQFB_PAN)
		RCSTR(MQFB_NAN)
		RCSTR(MQFB_STOPPED_BY_CHAD_EXIT)
		RCSTR(MQFB_STOPPED_BY_PUBSUB_EXIT)
		RCSTR(MQFB_NOT_A_REPOSITORY_MSG)
		RCSTR(MQFB_BIND_OPEN_CLUSRCVR_DEL)
		RCSTR(MQFB_MAX_ACTIVITIES)
		RCSTR(MQFB_NOT_FORWARDED)
		RCSTR(MQFB_NOT_DELIVERED)
		RCSTR(MQFB_UNSUPPORTED_FORWARDING)
		RCSTR(MQFB_UNSUPPORTED_DELIVERY)
		RCSTR(MQFB_DATA_LENGTH_ZERO)
		RCSTR(MQFB_DATA_LENGTH_NEGATIVE)
		RCSTR(MQFB_DATA_LENGTH_TOO_BIG)
		RCSTR(MQFB_BUFFER_OVERFLOW)
		RCSTR(MQFB_LENGTH_OFF_BY_ONE)
		RCSTR(MQFB_IIH_ERROR)
		RCSTR(MQFB_NOT_AUTHORIZED_FOR_IMS)
		RCSTR(MQFB_IMS_ERROR)
		RCSTR(MQFB_IMS_FIRST)
		RCSTR(MQFB_IMS_LAST)
		RCSTR(MQFB_CICS_INTERNAL_ERROR)
		RCSTR(MQFB_CICS_NOT_AUTHORIZED)
		RCSTR(MQFB_CICS_BRIDGE_FAILURE)
		RCSTR(MQFB_CICS_CORREL_ID_ERROR)
		RCSTR(MQFB_CICS_CCSID_ERROR)
		RCSTR(MQFB_CICS_ENCODING_ERROR)
		RCSTR(MQFB_CICS_CIH_ERROR)
		RCSTR(MQFB_CICS_UOW_ERROR)
		RCSTR(MQFB_CICS_COMMAREA_ERROR)
		RCSTR(MQFB_CICS_APPL_NOT_STARTED)
		RCSTR(MQFB_CICS_APPL_ABENDED)
		RCSTR(MQFB_CICS_DLQ_ERROR)
		RCSTR(MQFB_CICS_UOW_BACKED_OUT)
		RCSTR(MQFB_PUBLICATIONS_ON_REQUEST)
		RCSTR(MQFB_SUBSCRIBER_IS_PUBLISHER)
		RCSTR(MQFB_MSG_SCOPE_MISMATCH)
		RCSTR(MQFB_SELECTOR_MISMATCH)
		RCSTR(MQFB_NOT_A_GROUPUR_MSG)
		RCSTR(MQRC_PUT_INHIBITED)
		RCSTR(MQRC_Q_FULL)
		RCSTR(MQRC_NOT_AUTHORIZED)
		RCSTR(MQRC_Q_SPACE_NOT_AVAILABLE)
		RCSTR(MQRC_PERSISTENT_NOT_ALLOWED)
		RCSTR(MQRC_MSG_TOO_BIG_FOR_Q_MGR)
		RCSTR(MQRC_MSG_TOO_BIG_FOR_Q)
	)
;


JSONMessage::JSONMessage(Message::Ptr msg) : _msg(msg)
{
}

JSONMessage::~JSONMessage()
{
}

Poco::JSON::Object::Ptr JSONMessage::toJSONMQMD() const
{
	Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();

	std::string report = _reportCodes.getTextForValue(0, _msg->getReport());
	obj->set("Report", report);

	std::string msgType = _messageTypeCodes.getTextForValue(0, _msg->getMsgType());
	if (msgType.empty())
	{
		obj->set("MsgType", _msg->getMsgType());
	}
	else
	{
		obj->set("MsgType", msgType);
	}

	obj->set("Expiry", _msg->getExpiry());
	obj->set("Feedback", _feedbackCodes.getTextForValue(0, _msg->getFeedback()));
	obj->set("Encoding", _msg->getEncoding());
	obj->set("CodedCharSetId", _msg->getCodedCharSetId());
	obj->set("Format", Poco::trimRight(_msg->getFormat()));
	obj->set("Priority", _msg->getPriority());
	obj->set("Persistence", _msg->getPersistence());
	obj->set("MsgId", _msg->messageId()->toHex());
	obj->set("CorrelId", _msg->correlationId()->toHex());
	obj->set("BackoutCount", _msg->getBackoutCount());
	obj->set("ReplyToQ", _msg->getReplyToQueue());
	obj->set("ReplyToQmgr", _msg->getReplyToQMgr());
	obj->set("UserIdentifier", _msg->getUser());
	obj->set("AccountingToken", _msg->accountingToken()->toHex());
	obj->set("ApplIdentityData", _msg->getApplIdentityData());

	const TextMap& applTypes = PCFCommand::getTextMap("QueueStatus", MQIA_APPL_TYPE);
	TextMap::const_iterator it = applTypes.find(_msg->getPutApplType());
	obj->set("PutApplType", it == applTypes.end() ? "" : it->second);

	obj->set("PutApplName", _msg->getPutApplName());
	obj->set("PutDatetime", Poco::DateTimeFormatter::format(_msg->getPutDatetime(), "%Y-%m-%d %H:%M:%S.%i"));
	obj->set("PutDate", _msg->getPutDate());
	obj->set("PutTime", _msg->getPutTime());
	obj->set("ApplOriginData", _msg->getApplOriginData());
	obj->set("GroupId", _msg->groupId()->toHex());
	obj->set("MsgSeqNumber", _msg->getMsgSeqNumber());
	obj->set("Offset", _msg->getOffset());
	obj->set("MsgFlags", _msg->getMsgFlags());
	obj->set("OriginalLength", _msg->getOriginalLength());
	obj->set("Length", _msg->dataLength());

	return obj;
}

void JSONMessage::toMQMD(const Poco::JSON::Object& obj)
{
	// Set the putdate to the current timestamp to avoid an empty putdate when
	// it is not set in the MD JSON object
	Poco::DateTime now;
	_msg->setPutDatetime(now);

	if (obj.has("Report"))
	{
		MQLONG reportCode = _reportCodes.getIdForText(0, obj.optValue<std::string>("Report", ""));
		if (reportCode != -1)
		{
			_msg->setReport(reportCode);
		}
	}

	if (obj.has("Expiry")) _msg->setExpiry(obj.optValue<MQLONG>("Expiry", -1));

	if (obj.has("Feedback"))
	{
		MQLONG feedbackCode = _feedbackCodes.getIdForText(0, obj.optValue<std::string>("Feedback", ""));
		if (feedbackCode != -1)
		{
			_msg->setFeedback(feedbackCode);
		}
	}

	if (obj.has("Encoding")) _msg->setEncoding(obj.optValue<MQLONG>("Encoding", MQENC_NATIVE));
	if (obj.has("CodedCharSetId")) _msg->setCodedCharSetId(obj.optValue<MQLONG>("CodedCharSetId", MQCCSI_Q_MGR));
	if (obj.has("Format")) _msg->setFormat(obj.optValue<std::string>("Format", ""));
	if (obj.has("Priority")) _msg->setPriority(obj.optValue<MQLONG>("Priority", 0));
	if (obj.has("Persistence"))
	{
		bool persistency = obj.optValue<bool>("Persistence", false);
		_msg->setPersistence(persistency);
	}
	if (obj.has("MsgId"))
	{
		std::string hex = obj.optValue<std::string>("MsgId", "");
		_msg->messageId()->fromHex(hex);
	}
	if (obj.has("CorrelId"))
	{
		std::string hex = obj.optValue<std::string>("CorrelId", "");
		_msg->correlationId()->fromHex(hex);
	}
	if (obj.has("BackoutCount")) _msg->setBackoutCount(obj.optValue<MQLONG>("BackoutCount", 0));
	if (obj.has("ReplyToQ")) _msg->setReplyToQueue(obj.optValue<std::string>("ReplyToQ", ""));
	if (obj.has("ReplyToQMgr")) _msg->setReplyToQMgr(obj.optValue<std::string>("ReplyToQMgr", ""));
	if (obj.has("UserIdentifier")) _msg->setUser(obj.optValue<std::string>("UserIdentifier", ""));
	if (obj.has("AccountToken"))
	{
		std::string hex = obj.optValue<std::string>("AccountToken", "");
		_msg->accountingToken()->fromHex(hex);
	}
	if (obj.has("ApplIdentityData")) _msg->setApplIdentityData(obj.optValue<std::string>("ApplIdentityData", ""));
	if (obj.has("PutApplType"))
	{
		Poco::SharedPtr<Dictionary> dictionary = PCFCommand::dictionary("QueueStatus");
		MQLONG id = dictionary->getIdForText(MQIA_APPL_TYPE, obj.optValue<std::string>("PutApplType", ""));
		if (id == -1) // User defined?
		{
			_msg->setPutApplType(obj.optValue<MQLONG>("PutApplType", MQAT_DEFAULT));
		}
		else
		{
			_msg->setPutApplType(obj.optValue<MQLONG>("PutApplType", id));
		}
	}
	if (obj.has("PutApplName")) _msg->setPutApplName(obj.optValue<std::string>("PutApplName", ""));
	if (obj.has("PutDate")) _msg->setPutDate(obj.optValue<std::string>("PutDate", ""));
	if (obj.has("PutTime")) _msg->setPutDate(obj.optValue<std::string>("PutTime", ""));
	if (obj.has("PutDatetime"))
	{
		int tzd;
		try
		{
			Poco::DateTime dateTime = Poco::DateTimeParser::parse("%Y-%m-%d %H:%M:%S.%i", obj.optValue<std::string>("PutDatetime", ""), tzd);
			_msg->setPutDatetime(dateTime, tzd);
		}
		catch (Poco::SyntaxException&)
		{
		}
	}
	if (obj.has("ApplOriginData")) _msg->setApplOriginData(obj.optValue<std::string>("ApplOriginData", ""));

	// MQMQ_VERSION_2 fields
	if (obj.has("GroupId"))
	{
		std::string hex = obj.optValue<std::string>("GroupId", "");
		_msg->groupId()->fromHex(hex);
	}
	if (obj.has("MsgFlags")) _msg->setMsgFlags(obj.optValue<MQLONG>("MsgFlags", 0));
	if (obj.has("MsgSeqNumber")) _msg->setMsgSeqNumber(obj.optValue<MQLONG>("MsgSeqNumber", 0));
	if (obj.has("MsgType"))
	{
		MQLONG msgTypeCode = _messageTypeCodes.getIdForText(0, obj.optValue<std::string>("MsgType", ""));
		if (msgTypeCode != -1)
		{
			_msg->setMsgType(msgTypeCode);
		}
	}
	if (obj.has("Offset")) _msg->setOffset(obj.optValue<MQLONG>("Offset", 0));
	if (obj.has("OriginalLength")) _msg->setOriginalLength(obj.optValue<MQLONG>("OriginalLength", 0));
}


void JSONMessage::toJSON(Poco::JSON::Object::Ptr jsonMessage)
{
	if (_msg->getFormat().compare(MQFMT_DEAD_LETTER_HEADER) == 0)
	{
		if (_msg->buffer().size() >= sizeof(MQDLH))
		{
			MQDLH* dlh = (MQDLH*)_msg->buffer().data();
			Poco::JSON::Object::Ptr jsonDLH = new Poco::JSON::Object();
			jsonMessage->set("dlh", jsonDLH);
			jsonDLH->set("Version", dlh->Version);

			Poco::JSON::Object::Ptr jsonReason = new Poco::JSON::Object();
			jsonDLH->set("Reason", jsonReason);
			jsonReason->set("code", dlh->Reason);
			std::string reasonCodeStr = PCFCommand::getReasonString(dlh->Reason);
			jsonReason->set("desc", reasonCodeStr);

			jsonDLH->set("DestQName", Poco::trimRight(std::string(dlh->DestQName, MQ_Q_NAME_LENGTH)));
			jsonDLH->set("DestQMgrName", Poco::trimRight(std::string(dlh->DestQMgrName, MQ_Q_MGR_NAME_LENGTH)));
			jsonDLH->set("Encoding", dlh->Encoding);
			jsonDLH->set("CodedCharSetId", dlh->CodedCharSetId);
			jsonDLH->set("Format", Poco::trimRight(std::string(dlh->Format, MQ_FORMAT_LENGTH)));
			const TextMap& applTypes = PCFCommand::getTextMap("QueueStatus", MQIA_APPL_TYPE);
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
	else if (_msg->getFormat().compare(MQFMT_EVENT) == 0)
	{
		PCF pcfEvent(_msg);
		const PCFParameters& parameters = pcfEvent.getParameters();

		Poco::JSON::Object::Ptr jsonEvent = new Poco::JSON::Object();
		jsonMessage->set("event", jsonEvent);
		Poco::JSON::Object::Ptr jsonReason = new Poco::JSON::Object();
		jsonEvent->set("Reason", jsonReason);
		jsonReason->set("code", pcfEvent.getReasonCode());
		std::string reasonCodeStr = PCFCommand::getReasonString(pcfEvent.getReasonCode());
		jsonReason->set("desc", reasonCodeStr);

		if (pcfEvent.hasParameter(MQIACF_OBJECT_TYPE))
		{
			Poco::SharedPtr<Dictionary> dictionary;
			switch (parameters.getNumber(MQIACF_OBJECT_TYPE))
			{
			case MQOT_Q_MGR: dictionary = PCFCommand::dictionary("QueueManager"); break;
			case MQOT_CHANNEL:  dictionary = PCFCommand::dictionary("Channel"); break;
			case MQOT_NAMELIST: dictionary = PCFCommand::dictionary("Namelist"); break;
			case MQOT_PROCESS: dictionary = PCFCommand::dictionary("Process"); break;
			case MQOT_Q: dictionary = PCFCommand::dictionary("Queue"); break;
			case MQOT_LISTENER: dictionary = PCFCommand::dictionary("Listener"); break;
			case MQOT_AUTH_REC: dictionary = PCFCommand::dictionary("AuthorityRecord"); break;
			case MQOT_CHLAUTH: dictionary = PCFCommand::dictionary("ChannelAuthenticationRecord"); break;
			default:
				Poco::Logger::get("mq.web").warning("No dictionary set for event. ObjectType %d", parameters.getNumber(MQIACF_OBJECT_TYPE));
			}
			poco_assert_msg_dbg(!dictionary.isNull(), "Dictionary missing");
			if (!dictionary.isNull())
			{
				dictionary->mapToJSON(parameters, jsonEvent, false);
			}
		}
		Poco::SharedPtr<Dictionary> dictionary = PCFCommand::dictionary("Event");
		poco_assert_dbg(!dictionary.isNull());
		dictionary->mapToJSON(parameters, jsonEvent, false);
	}
	else if (_msg->getFormat().compare(MQFMT_ADMIN) == 0)
	{
		PCF pcfAdmin(_msg);
		Poco::JSON::Object::Ptr jsonAdmin = new Poco::JSON::Object();
		jsonMessage->set("admin", jsonAdmin);
		jsonAdmin->set("command", PCFCommand::getCommandString(pcfAdmin.getCommand()));
		Poco::JSON::Object::Ptr reason = new Poco::JSON::Object();
		jsonAdmin->set("reason", reason);
		reason->set("code", pcfAdmin.getReasonCode());
		reason->set("desc", PCFCommand::getReasonString(pcfAdmin.getReasonCode()));

		Poco::JSON::Object::Ptr jsonParameters = new Poco::JSON::Object();
		jsonAdmin->set("parameters", jsonParameters);

		Poco::SharedPtr<Dictionary> dictionary;
		switch (pcfAdmin.getCommand())
		{
		case MQCMD_ACCOUNTING_MQI:
			dictionary = PCFCommand::dictionary("AccountingMQI");
			break;
		case MQCMD_ACCOUNTING_Q:
			dictionary = PCFCommand::dictionary("AccountingQueue");
			break;
		case MQCMD_STATISTICS_MQI:
			dictionary = PCFCommand::dictionary("StatisticsMQI");
			break;
		case MQCMD_STATISTICS_Q:
		{
			dictionary = PCFCommand::dictionary("StatisticsQueue");
			break;
		}
		case MQCMD_STATISTICS_CHANNEL:
			dictionary = PCFCommand::dictionary("StatisticsChannel");
			break;
		}
		poco_assert_msg_dbg(!dictionary.isNull(), "Dictionary missing for statistics");
		if (!dictionary.isNull()) {
			const PCFParameters& parameters = pcfAdmin.getParameters();
			dictionary->mapToJSON(parameters, jsonParameters, false);
		}
	}
	else if (_msg->getFormat().compare(MQFMT_STRING) == 0)
	{
		if (_msg->dataLength() < _msg->buffer().size())
		{
			_msg->buffer().resize(_msg->dataLength());
		}
		jsonMessage->set("data", _msg->buffer().toString());
	}
	else if (_msg->getFormat().compare(MQFMT_RF_HEADER_2) == 0)
	{
		MQBYTE* begin = (MQBYTE*)_msg->buffer().data();
		MQRFH2* rfh2 = (MQRFH2*)begin;
		Poco::JSON::Object::Ptr jsonRfh2 = new Poco::JSON::Object();
		jsonMessage->set("rfh2", jsonRfh2);
		jsonRfh2->set("Encoding", rfh2->Encoding);

		Poco::JSON::Array::Ptr jsonNameValues = new Poco::JSON::Array();
		jsonRfh2->set("NameValues", jsonNameValues);

		MQBYTE* data = begin + sizeof(MQRFH2);
		MQBYTE* end = begin + rfh2->StrucLength;

		while (data < end)
		{
			MQLONG len = *(MQLONG*)data;
			data += sizeof(MQLONG);

			if (len < 0 || data + len > end)
			{
				//TODO: invalid RFH2 ...
				break;
			}
			std::string value((const char*)data, len);
			if (value[0] == '\0') value.resize(0);
			Poco::trimRightInPlace(value);
			jsonNameValues->add(value);
			data += len;
		}
	}
}

}}