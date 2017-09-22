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
