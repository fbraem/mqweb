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
#include <sstream>
#include <iomanip>

#include <Poco/HexBinaryEncoder.h>
#include <Poco/HexBinaryDecoder.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>

#include <MQ/Message.h>

namespace MQ
{

MQMD Message::_initialMD = {MQMD_DEFAULT};


Message::Message(int size) 
	: _buffer(size)
	, _md(_initialMD)
	, _dataLength(0L)
{
}


std::string Message::getBufferAsHex(const MQBYTE* buffer, long size)
{
	std::ostringstream oss;

	Poco::HexBinaryEncoder hexEncoder(oss);
	hexEncoder.rdbuf()->setLineLength(0);
	hexEncoder.rdbuf()->setUppercase(true);
	hexEncoder.write((const char*) buffer, size);
	hexEncoder.close();

	return oss.str();
}


void Message::setBufferFromHex(MQBYTE* buffer, long size, const std::string& hex)
{
	std::istringstream iss(hex);
	Poco::HexBinaryDecoder decoder(iss);
	int c = decoder.get();
	int i = 0;
	while (c != -1 && i < size)
	{
		buffer[i++] = (unsigned char) c;
		c = decoder.get();
	}
}


void Message::copyBuffer(MQBYTE* target, const Buffer& buffer, long maxSize)
{
 	std::memset(target, 0, maxSize);
	std::memcpy(target, 
				buffer.begin(), 
				buffer.size() > maxSize ? maxSize : buffer.size());
}


bool Message::isEmptyBuffer(const MQBYTE* buffer, long size)
{
	for(long i = 0; i < size; i++)
	{
		if ( buffer[i] != 0 )
		{
			return false;
		}
	}
	return true;
}


Poco::DateTime Message::getPutDate() const
{
	Poco::DateTime dateTime;

	std::string dateValue(_md.PutDate, MQ_PUT_DATE_LENGTH);
	std::string timeValue(_md.PutTime, MQ_PUT_TIME_LENGTH);
	dateValue += timeValue;

	int timeZone;
	Poco::DateTimeParser::parse("%Y%n%e%H%M%S", dateValue, dateTime, timeZone);
	return dateTime;
}

}
