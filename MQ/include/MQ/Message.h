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
{
public:
	Message(int size = 0);
	
	
	Buffer& buffer();
	
	
	const Buffer& buffer() const;
	
	
	void clear();
	
	
	BufferPtr getCorrelationId() const;
	
	
	void setCorrelationId(const Buffer& buffer);
	
	
	MQLONG getCodedCharSetId() const;


	void setCodedCharSetId(MQLONG ccsid);

	
	MQLONG getEncoding() const;
	
	
	void setEncoding(MQLONG encoding);
	
	
	std::string getFormat() const;
	
	
	void setFormat(const std::string& format);


	BufferPtr getMessageId() const;
	
	
	bool isEmptyMessageId() const;
	
	
	void setMessageId(const Buffer& buffer);
	

	MQLONG persistence() const;


	void setPersistence(MQLONG persistence);

	
	std::string getReplyToQueue() const;
	
	
	void setReplyToQueue(const std::string& queue);
	

	MQLONG getType() const;


	void setType(MQLONG type);
	
	
	Poco::DateTime getPutDate() const;
	
	
	std::string getPutApplication() const;
	
	
	std::string getUser() const;
	
	
	MQLONG dataLength() const;
	

private:
	
	Buffer _buffer;
	

	MQMD _md;
	

	// Contains the length of the message on the queue
	// Only valid when the message is browsed or retrieved.
	MQLONG _dataLength;
	
	
	MQMD* md();


	static MQMD _initialMD;


	friend class Queue;

};


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
}


inline MQLONG Message::getCodedCharSetId() const
{
	return _md.CodedCharSetId;
}


inline void Message::setCodedCharSetId(MQLONG ccsid)
{
	_md.CodedCharSetId = ccsid;
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


inline void Message::setType(MQLONG type)
{
	_md.MsgType = type;
}

inline MQLONG Message::getType() const
{
	return _md.MsgType;
}


inline MQLONG Message::dataLength() const
{
	return _dataLength;
}

} // namespace MQ

#endif // _MQ_Message_h
