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

#ifndef _MQ_MQException_h
#define _MQ_MQException_h

#include <string>

#include <Poco/Logger.h>

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
