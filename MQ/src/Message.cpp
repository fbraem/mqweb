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

#include "Poco/HexBinaryEncoder.h"
#include "Poco/HexBinaryDecoder.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/NumberFormatter.h"

#include "MQ/Message.h"

namespace MQ
{

MQMD Message::_initialMD = {MQMD_DEFAULT};


Message::Message(int size) 
	: _buffer(size)
	, _md(_initialMD)
	, _dataLength(0L)
{
}

Message::Message(const MQBYTE* buffer, MQLONG size)
	: _buffer(buffer, size)
{
}

void Message::setPutDatetime(const Poco::DateTime &putDate, int tz)
{
	setPutDate(Poco::DateTimeFormatter::format(putDate, "%Y%m%d", tz));

	int ms = putDate.millisecond();
	ms /= 10; // Put time is hundredths of seconds

	std::string time = Poco::DateTimeFormatter::format(putDate, "%H%M%S", tz);
	time += Poco::NumberFormatter::format0(ms, 2);
	strncpy(_md.PutTime, time.c_str(), MQ_PUT_TIME_LENGTH);
}

Poco::DateTime Message::createDatetime(const std::string& date, const std::string& time)
{
	Poco::DateTime dateTime;

	std::string dateValue = date;
	dateValue += time;
	dateValue += "0"; // Put time is hundredths of seconds

	int timeZone;
	Poco::DateTimeParser::parse("%Y%n%e%H%M%S%i", dateValue, dateTime, timeZone);
	return dateTime;
}


}
