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
#ifndef _MQ_MQException_h
#define _MQ_MQException_h

#include <string>

#include "Poco/Logger.h"

namespace MQ {

class MQException : public Poco::Exception
	/// Exception thrown when there is a problem with Websphere MQ
{
public:
	MQException(const std::string& object, const std::string& function, long code, long reason);
		/// Constructor

	virtual ~MQException() throw() { }
		/// Destructor

	void object(const std::string& object);
		/// Sets the Websphere MQ object related with this exception.

	std::string object() const;
		/// If known, it returns the Websphere MQ object related with this exception.

	std::string function() const;
		/// Returns the Websphere MQ function that resulted in this exception (MQPUT, MQGET, ...)

	long code() const;
		/// Returns the completion code of the last executed Websphere MQ function.

	long reason() const;
		/// Returns the reason code of the last executed Websphere MQ function.


	void log(Poco::Logger& logger) const;
		/// Log this exception to the given logger

	virtual const char* name() const throw();


	virtual const char* what() const throw();

private:
	std::string _object;

	std::string _function;

	long _code;

	long _reason;

};


inline void MQException::object(const std::string& object)
{
	_object = object;
}

inline std::string MQException::object() const
{
	return _object;
}


inline std::string MQException::function() const
{
	return _function;
}

inline long MQException::code() const
{
	return _code;
}

inline long MQException::reason() const
{
	return _reason;
}

} // namespace MQ

#endif // _MQ_MQException_h
