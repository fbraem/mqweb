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
