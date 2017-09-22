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
#ifndef _MQ_Message_h
#define _MQ_Message_h

#include <cmqc.h>

#include <string.h>

#include "Poco/String.h"
#include "Poco/SharedPtr.h"
#include "Poco/Buffer.h"
#include "Poco/DateTimeFormatter.h"

#include "MQ/Buffer.h"

namespace MQ {


class Message
	/// Represenst a Websphere MQ Message
{
public:
	Message(int size = 0);
		/// Constructor. Creates a message with a buffer of the given size.

	explicit Message(const MQBYTE* buffer, MQLONG size);
		/// Constructor. Note: Message doesn't own the buffer. In this case
		/// the Buffer class only acts as a wrapper around the externally
		/// supplied memory.

	Buffer& buffer();
		/// Returns the buffer used for storing the content of the message.

	const Buffer& buffer() const;
		/// Returns a const reference to the content of the message.

	void clear();
		/// Clears the content and the fields of the message.

	Buffer::Ptr accountingToken();
		/// Returns the account token as a Buffer. The buffer
		/// doesn't own the memory which means: you can't resize it
		/// but you can change the content.

	Buffer::Ptr accountingToken() const;
		/// Returns the account token as a Buffer. This is the
		/// const version. The buffer is a copy, which means
		/// you can resize and change it, but the orginal account
		/// token will not be changed.

	std::string getApplIdentityData() const;
		/// Gets the application data relating to identity

	void setApplIdentityData(const std::string& applIdentity);
		/// Sets the application data relating to identity

	std::string getApplOriginData() const;
		/// Gets the application data relating to origin

	void setApplOriginData(const std::string& applOriginData);
		/// Sets the application data relating to origin

	MQLONG getBackoutCount() const;
		/// Returns the backout counter

	void setBackoutCount(MQLONG count);
		/// Sets the backout counter

	Buffer::Ptr correlationId();
		/// Returns the correlation id as a Buffer. The buffer
		/// doesn't own the memory which means: you can't resize it
		/// but you can change the content.

	Buffer::Ptr correlationId() const;
		/// Returns the correlation id as a Buffer. This is the
		/// const version. The buffer is a copy, which means
		/// you can resize and change it, but the orginal correlation
		/// id will not be changed.

	MQLONG getCodedCharSetId() const;
		/// Returns the coded character set id

	void setCodedCharSetId(MQLONG ccsid);
		/// Sets the coded character set id

	MQLONG dataLength() const;
		/// Returns the data length of the message

	MQLONG getEncoding() const;
		/// Gets the encoding

	void setEncoding(MQLONG encoding);
		/// Sets the encoding

	MQLONG getExpiry() const;
		/// Gets the message lifetime

	void setExpiry(MQLONG expiry);
		/// Sets the message lifetime

	MQLONG getFeedback() const;
		/// Get feedback or reason code

	void setFeedback(MQLONG feedback);
		/// Set feedback or reason code

	std::string getFormat() const;
		/// Gets the format

	void setFormat(const std::string& format);
		/// Sets the format

	Buffer::Ptr groupId();
		/// Returns the group id as a Buffer. The buffer
		/// doesn't own the memory which means: you can't resize it
		/// but you can change the content.

	Buffer::Ptr groupId() const;
		/// Returns the group id as a Buffer. This is the
		/// const version. The buffer is a copy, which means
		/// you can resize and change it, but the orginal group
		/// id will not be changed.

	MQLONG getMsgFlags() const;
		/// Gets the message flags

	void setMsgFlags(MQLONG flags);
		/// Sets the message flags

	Buffer::Ptr messageId();
		/// Returns the message id as a Buffer. The buffer
		/// doesn't own the memory which means: you can't resize it
		/// but you can change the content.

	Buffer::Ptr messageId() const;
		/// Returns the message id as a Buffer. This is the
		/// const version. The buffer is a copy, which means
		/// you can resize and change it, but the orginal message
		/// id will not be changed.

	MQLONG getMsgSeqNumber() const;
		/// Gets the sequence number of logical message within group

	void setMsgSeqNumber(MQLONG seqNumber);
		/// Sets the sequence number of logical message within group

	MQLONG getMsgType() const;
		/// Gets the message type

	void setMsgType(MQLONG type);
		/// Sets the message type

	MQLONG getOffset() const;
		/// Gets the offset of data in physical message from start of logical message

	void setOffset(MQLONG offset);
		/// Sets the offset of data in physical message from start of logical message

	MQLONG getOriginalLength() const;
		/// Gets length of original message

	void setOriginalLength(MQLONG length);
		/// Sets length of original message

	static Poco::DateTime createDatetime(const std::string& date, const std::string& time);
		/// Creates a DateTime from a MQ date and MQ time

	MQLONG getPersistence() const;
		/// Returns the persistence flag

	void setPersistence(MQLONG persistence);
		/// Sets the persistence flag

	MQLONG getPriority() const;
		/// Returns the message priority

	void setPriority(MQLONG prio);
		/// Sets the message priority

	std::string getPutApplName() const;
		/// Returns the name of application that put the message

	void setPutApplName(const std::string& app);
		/// Sets the name of application that put the message

	MQLONG getPutApplType() const;
		/// Gets the type of application that put the message

	void setPutApplType(MQLONG appType);
		/// Sets the type of application that put the message

	std::string getPutDate() const;
		/// Returns the put date

	Poco::DateTime getPutDatetime() const;
	/// Returns the put datetime (incl. time)

	void setPutDate(const std::string& date);
		/// Sets the put date. Date format must be YYYYMMDD where month is 01 through 12
		/// and day of month is 01 through 31.

	void setPutDatetime(const Poco::DateTime& putDate, int tz = Poco::DateTimeFormatter::UTC);
		/// Sets the put date and time

	std::string getPutTime() const;
		/// Returns the put time

	void setPutTime(const std::string& date);
	/// Sets the put time. Time format must be HHMMSSTH.

	std::string getReplyToQMgr() const;
		/// Returns the name of the reply queue manager

	void setReplyToQMgr(const std::string& qmgr);
		/// Sets the name of reply queue manager

	std::string getReplyToQueue() const;
		/// Returns the name of the reply-to-queue

	void setReplyToQueue(const std::string& queue);
		/// Sets the name of the reply-to-queue

	MQLONG getReport() const;
		/// Get the report attribute

	void setReport(MQLONG report);
		/// Set the report attribute

	MQLONG getType() const;
		/// Returns the type of the message

	void setType(MQLONG type);
		/// Sets the type of the message

	std::string getUser() const;
		/// Returns the name of the user

	void setUser(const std::string& user);
		/// Sets the name of the user

	typedef Poco::SharedPtr<Message> Ptr;

private:

	Buffer _buffer;

	MQMD _md;

	// Contains the length of the message on the queue
	// Only valid when the message is browsed or retrieved.
	MQLONG _dataLength;

	MQMD* md();

	static MQMD _initialMD;

	friend class Queue;

	friend class MessageConsumer;

	friend class Topic;
};


inline Buffer::Ptr Message::accountingToken()
{
	return new Buffer(_md.AccountingToken, MQ_ACCOUNTING_TOKEN_LENGTH);
}


inline Buffer::Ptr Message::accountingToken() const
{
	return new Buffer(_md.AccountingToken, MQ_ACCOUNTING_TOKEN_LENGTH);
}


inline std::string Message::getApplIdentityData() const
{
	return Poco::trimRight(std::string(_md.ApplIdentityData, MQ_APPL_IDENTITY_DATA_LENGTH));
}


inline void Message::setApplIdentityData(const std::string& applIdentity)
{
	strncpy(_md.ApplIdentityData, applIdentity.c_str(), MQ_APPL_IDENTITY_DATA_LENGTH);
}


inline std::string Message::getApplOriginData() const
{
	return Poco::trimRight(std::string(_md.ApplOriginData, MQ_APPL_ORIGIN_DATA_LENGTH));
}


inline void Message::setApplOriginData(const std::string& applOriginData)
{
	strncpy(_md.ApplOriginData, applOriginData.c_str(), MQ_APPL_ORIGIN_DATA_LENGTH);
}


inline MQLONG Message::getBackoutCount() const
{
	return _md.BackoutCount;
}

inline void Message::setBackoutCount(MQLONG count)
{
	_md.BackoutCount = count;
}

inline Buffer& Message::buffer()
{
	return _buffer;
}

inline const Buffer& Message::buffer() const
{
	return _buffer;
}

inline void Message::clear()
{
	_md = _initialMD;
	_buffer.clear();
}


inline MQLONG Message::getCodedCharSetId() const
{
	return _md.CodedCharSetId;
}


inline void Message::setCodedCharSetId(MQLONG ccsid)
{
	_md.CodedCharSetId = ccsid;
}


inline Buffer::Ptr Message::correlationId()
{
	return new Buffer(_md.CorrelId, MQ_CORREL_ID_LENGTH);
}


inline Buffer::Ptr Message::correlationId() const
{
	return new Buffer(_md.CorrelId, MQ_CORREL_ID_LENGTH);
}


inline MQLONG Message::dataLength() const
{
	return _dataLength;
}


inline MQLONG Message::getEncoding() const
{
	return _md.Encoding;
}


inline void Message::setEncoding(MQLONG encoding)
{
	_md.Encoding = encoding;
}


inline MQLONG Message::getExpiry() const
{
	return _md.Expiry;
}


inline void Message::setExpiry(MQLONG expiry)
{
	_md.Expiry = expiry;
}


inline MQLONG Message::getFeedback() const
{
	return _md.Feedback;
}


inline void Message::setFeedback(MQLONG feedback)
{
	_md.Feedback = feedback;
}


inline std::string Message::getFormat() const
{
	std::string s(_md.Format, MQ_FORMAT_LENGTH);
	return s;
}

inline void Message::setFormat(const std::string& format)
{
	strncpy(_md.Format, format.c_str(), MQ_FORMAT_LENGTH);
}


inline MQMD* Message::md()
{
	return &_md;
}


inline Buffer::Ptr Message::groupId()
{
	_md.Version = MQMD_VERSION_2;
	return new Buffer(_md.GroupId, MQ_GROUP_ID_LENGTH);
}


inline Buffer::Ptr Message::groupId() const
{
	return new Buffer(_md.GroupId, MQ_GROUP_ID_LENGTH);
}


inline MQLONG Message::getMsgFlags() const
{
	return _md.MsgFlags;
}


inline void Message::setMsgFlags(MQLONG flags)
{
	_md.Version = MQMD_VERSION_2;
	_md.MsgFlags = flags;
}


inline Buffer::Ptr Message::messageId()
{
	return new Buffer(_md.MsgId, MQ_MSG_ID_LENGTH);
}


inline Buffer::Ptr Message::messageId() const
{
	return new Buffer(_md.MsgId, MQ_MSG_ID_LENGTH);
}


inline MQLONG Message::getMsgSeqNumber() const
{
	return _md.MsgSeqNumber;
}


inline void Message::setMsgSeqNumber(MQLONG seqNumber)
{
	_md.Version = MQMD_VERSION_2;
	_md.MsgSeqNumber = seqNumber;
}


inline MQLONG Message::getOffset() const
{
	return _md.Offset;
}


inline void Message::setOffset(MQLONG offset)
{
	_md.Version = MQMD_VERSION_2;
	_md.Offset = offset;
}


inline MQLONG Message::getOriginalLength() const
{
	return _md.OriginalLength;
}

inline void Message::setOriginalLength(MQLONG length)
{
	_md.Version = MQMD_VERSION_2;
	_md.OriginalLength = length;
}

inline MQLONG Message::getMsgType() const
{
	return _md.MsgType;
}


inline void Message::setMsgType(MQLONG type)
{
	_md.MsgType = type;
}


inline MQLONG Message::getPersistence() const
{
	return _md.Persistence;
}


inline void Message::setPersistence(MQLONG persistence)
{
	_md.Persistence = persistence;
}


inline MQLONG Message::getPriority() const
{
	return _md.Priority;
}


inline void Message::setPriority(MQLONG prio)
{
	_md.Priority = prio;
}


inline std::string Message::getPutApplName() const
{
	return Poco::trimRight(std::string(_md.PutApplName, MQ_PUT_APPL_NAME_LENGTH));
}


inline void Message::setPutApplName(const std::string& app)
{
	strncpy(_md.PutApplName, app.c_str(), MQ_PUT_APPL_NAME_LENGTH);
}


inline MQLONG Message::getPutApplType() const
{
	return _md.PutApplType;
}


inline void Message::setPutApplType(MQLONG applType)
{
	_md.PutApplType = applType;
}

inline Poco::DateTime Message::getPutDatetime() const
{
	return createDatetime(getPutDate(), getPutTime());
}

inline void Message::setPutDate(const std::string& date)
{
	strncpy(_md.PutDate, date.c_str(), MQ_PUT_DATE_LENGTH);
}

inline std::string Message::getPutDate() const
{
	return Poco::trimRight(std::string(_md.PutDate, MQ_PUT_DATE_LENGTH));
}

inline void Message::setPutTime(const std::string& time)
{
	strncpy(_md.PutTime, time.c_str(), MQ_PUT_TIME_LENGTH);
}

inline std::string Message::getPutTime() const
{
	return Poco::trimRight(std::string(_md.PutTime, MQ_PUT_TIME_LENGTH));
}

inline void Message::setReplyToQMgr(const std::string& qmgr)
{
	strncpy(_md.ReplyToQMgr, qmgr.c_str(), MQ_Q_MGR_NAME_LENGTH);
}


inline std::string Message::getReplyToQMgr() const
{
	return Poco::trimRight(std::string(_md.ReplyToQMgr, MQ_Q_MGR_NAME_LENGTH));
}


inline void Message::setReplyToQueue(const std::string& queue)
{
	strncpy(_md.ReplyToQ, queue.c_str(), MQ_Q_NAME_LENGTH);
}


inline std::string Message::getReplyToQueue() const
{
	return Poco::trimRight(std::string(_md.ReplyToQ, MQ_Q_NAME_LENGTH));
}


inline MQLONG Message::getReport() const
{
	return _md.Report;
}


inline void Message::setReport(MQLONG report)
{
	_md.Report = report;
}


inline void Message::setType(MQLONG type)
{
	_md.MsgType = type;
}

inline MQLONG Message::getType() const
{
	return _md.MsgType;
}

inline std::string Message::getUser() const
{
	return Poco::trimRight(std::string(_md.UserIdentifier, MQ_USER_ID_LENGTH));
}

inline void Message::setUser(const std::string& user)
{
	strncpy(_md.UserIdentifier, user.c_str(), MQ_USER_ID_LENGTH);
}

} // namespace MQ

#endif // _MQ_Message_h
