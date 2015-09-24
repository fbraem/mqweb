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
#include <cmqc.h>

#include "Poco/HexBinaryDecoder.h"
#include "Poco/HexBinaryEncoder.h"

#include "MQ/Buffer.h"

namespace MQ {

Buffer::Buffer(size_t size) : _buffer(size)
{
}

Buffer::Buffer(MQBYTE* bytes, std::size_t size) : _buffer(bytes, size)
{
}

Buffer::Buffer(const MQBYTE* bytes, std::size_t size) : _buffer(bytes, size)
{
}

Buffer::Buffer(const std::string& hex) : _buffer(hex.size() / 2)
{
	fromHex(hex);
}

Buffer::~Buffer()
{
}

void Buffer::fromHex(const std::string& hex)
{
	std::istringstream iss(hex);
	Poco::HexBinaryDecoder decoder(iss);
	int c = decoder.get();
	int i = 0;
	while (c != -1 && i < _buffer.size())
	{
		_buffer[i++] = (unsigned char) c;
		c = decoder.get();
	}
}

std::string Buffer::toHex() const
{
	std::ostringstream oss;

	Poco::HexBinaryEncoder hexEncoder(oss);
	hexEncoder.rdbuf()->setLineLength(0);
	hexEncoder.rdbuf()->setUppercase(true);
	hexEncoder.write((const char*) _buffer.begin(), _buffer.size());
	hexEncoder.close();

	return oss.str();
}

}
