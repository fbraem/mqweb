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

#ifndef _MQ_Message_h
#define _MQ_Message_h

#include <cmqc.h>

#include <string.h>

#include <Poco/SharedPtr.h>
#include <Poco/Buffer.h>

#include <MQ/Buffer.h>

namespace MQ {


class Message
	/// Represenst a Websphere MQ Message
{
public:
	Message(int size = 0);
		/// Constructor. Creates a message with a buffer of the given size.
	
	
	Buffer& buffer();
		/// Returns the buffer used for storing the content of the message.
	
	
	const Buffer& buffer() const;
		/// Returns a const reference to the content of the message.
	
	
	void clear();
		/// Clears the content and the fields of the message.


	BufferPtr getAccountToken() const;


	std::string getAccountTokenHex() const;


	void setAccountToken(const Buffer& buffer);

	
	BufferPtr getCorrelationId() const;
		/// Returns a copy of the correlation id in a buffer
	

	std::string getCorrelationIdHex() const;
		/// Returns the correlation id in a hex format


	void setCorrelationId(const Buffer& buffer);
		/// Sets the correlation id with the content of the buffer


	void setCorrelationId(const std::string& hex);
		/// Sets the correlation id from a hex value

	
	bool isEmptyCorrelationId() const;
		/// Returns true when the correlation id contains all 0 bytes


	MQLONG getCodedCharSetId() const;
		/// Returns the coded character set id


	void setCodedCharSetId(MQLONG ccsid);
		/// Sets the coded character set id


	MQLONG dataLength() const;
		/// Returns the data length of the message


	MQLONG getEncoding() const;
		/// Gets the encoding
	
	
	void setEncoding(MQLONG encoding);
		///.Sets the encoding
	
	
	std::string getFormat() const;
		/// Gets the format
	
	
	void setFormat(const std::string& format);
		/// Sets the format


	BufferPtr getMessageId() const;
		/// Returns a copy of the message id as a Buffer


	std::string getMessageIdHex() const;
		/// Returns the message id in a hex format
	
	
	bool isEmptyMessageId() const;
		/// Returns true when the message id is initialized with 0 bytes
	
	
	void setMessageId(const Buffer& buffer);
		/// Sets the message id from a buffer


	void setMessageId(const std::string& hex);
		/// Sets the message id from a hex string


	MQLONG persistence() const;
		/// Returns the persistence flag


	void setPersistence(MQLONG persistence);
		/// Sets the persistence flag


	std::string getPutApplication() const;
		/// Returns the name of the put application


	Poco::DateTime getPutDate() const;
		/// Returns the put datetime


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


private:

	Buffer _buffer;


	MQMD _md;


	// Contains the length of the message on the queue
	// Only valid when the message is browsed or retrieved.
	MQLONG _dataLength;


	MQMD* md();


	static MQMD _initialMD;


	friend class Queue;


	static void copyBuffer(MQBYTE* target, const Buffer& buffer, long maxSize);


	static std::string getBufferAsHex(const MQBYTE* buffer, long size);


	static bool isEmptyBuffer(const MQBYTE* buffer, long size);


	static void setBufferFromHex(MQBYTE* buffer, long size, const std::string& hex);

};


inline std::string Message::getAccountTokenHex() const
{
	return getBufferAsHex(_md.AccountingToken, MQ_ACCOUNTING_TOKEN_LENGTH);
}


inline void Message::setAccountToken(const Buffer& buffer)
{
	copyBuffer(_md.AccountingToken, buffer, MQ_ACCOUNTING_TOKEN_LENGTH);
}

inline BufferPtr Message::getAccountToken() const
{
	return new Buffer(_md.AccountingToken, MQ_ACCOUNTING_TOKEN_LENGTH);
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


inline BufferPtr Message::getCorrelationId() const
{
	return new Buffer(_md.CorrelId, MQ_CORREL_ID_LENGTH);
}


inline std::string Message::getCorrelationIdHex() const
{
	return getBufferAsHex(_md.CorrelId, MQ_CORREL_ID_LENGTH);
}


inline MQLONG Message::dataLength() const
{
	return _dataLength;
}


inline bool Message::isEmptyCorrelationId() const
{
	return isEmptyBuffer(_md.CorrelId, MQ_CORREL_ID_LENGTH);
}


inline void Message::setCorrelationId(const Buffer& buffer)
{
	copyBuffer(_md.CorrelId, buffer, MQ_CORREL_ID_LENGTH);
}


inline void Message::setCorrelationId(const std::string& hex)
{
	setBufferFromHex(_md.CorrelId, MQ_CORREL_ID_LENGTH, hex);
}


inline MQLONG Message::getEncoding() const
{
	return _md.Encoding;
}


inline void Message::setEncoding(MQLONG encoding)
{
	_md.Encoding = encoding;
}


inline std::string Message::getFormat() const
{
	std::string s(_md.Format, MQ_FORMAT_LENGTH - 1);
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


inline BufferPtr Message::getMessageId() const
{
	return new Buffer(_md.MsgId, MQ_MSG_ID_LENGTH);
}


inline bool Message::isEmptyMessageId() const
{
	return isEmptyBuffer(_md.MsgId, MQ_MSG_ID_LENGTH);
}

inline void Message::setMessageId(const Buffer& buffer)
{
	copyBuffer(_md.MsgId, buffer, MQ_MSG_ID_LENGTH);
}


inline void Message::setMessageId(const std::string& hex)
{
	setBufferFromHex(_md.MsgId, MQ_MSG_ID_LENGTH, hex);
}

inline std::string Message::getMessageIdHex() const
{
	return getBufferAsHex(_md.MsgId, MQ_MSG_ID_LENGTH);
}

inline MQLONG Message::persistence() const
{
	return _md.Persistence;
}

inline void Message::setPersistence(MQLONG persistence)
{
	_md.Persistence = persistence;
}

inline void Message::setReplyToQueue(const std::string& queue)
{
	strncpy(_md.ReplyToQ, queue.c_str(), MQ_Q_NAME_LENGTH);
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


} // namespace MQ

#endif // _MQ_Message_h
