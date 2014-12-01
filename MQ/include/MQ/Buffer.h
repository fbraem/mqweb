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

#ifndef _MQ_Buffer_h
#define _MQ_Buffer_h

#include "Poco/Buffer.h"
#include "Poco/SharedPtr.h"

namespace MQ {

class Buffer
	/// Wrapper class for Poco::Buffer
{
public:

	typedef Poco::SharedPtr<Buffer> Ptr;

	Buffer(size_t size = 0);
		/// Constructor

	explicit Buffer(MQBYTE* bytes, std::size_t size);
		/// Constructor. Warning: The buffer doesn't own the memory!

	explicit Buffer(const MQBYTE* bytes, std::size_t size);
		/// Constructor. Note: Buffer will copy the memory and takes ownership!

	Buffer(const std::string& hex);
		/// Decodes a string in hex to a buffer

	virtual ~Buffer();
		/// Destructor

	unsigned char chr(std::size_t pos = 0) const;

	void clear();
		/// Clears the buffer

	void copyTo(MQBYTE* target, std::size_t size) const;

	unsigned char* data(std::size_t pos = 0);

	const unsigned char* data(std::size_t pos = 0) const;

	bool hasAllNullBytes() const;

	size_t size() const;

	std::string toString() const;

	void fromHex(const std::string& hex);

	std::string toHex() const;

	void resize(std::size_t sizen, bool preserveContent = true);

private:

	Buffer(const Buffer& );
	Buffer& operator =(const Buffer& other);

	Poco::Buffer<MQBYTE> _buffer;
};

inline unsigned char Buffer::chr(std::size_t pos) const
{
	poco_assert_dbg(pos < _buffer.size());
	return _buffer[pos];
}

inline void Buffer::clear()
{
	_buffer.clear();
}

inline void Buffer::copyTo(MQBYTE* target, std::size_t size) const
{
	std::memset(target, 0, size);
	std::memcpy(target, 
				_buffer.begin(), 
				_buffer.size() > size ? size : _buffer.size());
}

inline const unsigned char* Buffer::data(std::size_t pos) const
{
	poco_assert_dbg(pos < _buffer.size());
	return &_buffer[pos];
}

inline unsigned char* Buffer::data(std::size_t pos)
{
	poco_assert_dbg(pos < _buffer.size());
	return &_buffer[pos];
}

inline bool Buffer::hasAllNullBytes() const
{
	for(std::size_t i = 0; i < _buffer.size(); ++i)
	{
		if ( _buffer[i] != 0 )
		{
			return false;
		}
	}
	return true;
}

inline size_t Buffer::size() const
{
	return _buffer.size();
}

inline void Buffer::resize(std::size_t size, bool preserveContent)
{
	_buffer.resize(size, preserveContent);
}

inline std::string Buffer::toString() const
{
	return std::string((const char*) _buffer.begin(), _buffer.size());
}

} // namespace MQ

#endif // _MQ_Buffer_h
