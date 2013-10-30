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

#ifndef _MQ_PCF_h
#define _MQ_PCF_h

#include <cmqc.h>
#include <cmqcfc.h> /* PCF  */

#include <map>
#include <vector>

#include <Poco/DateTime.h>

#include <MQ/Message.h>

namespace MQ {

class PCF : public Message
	/// Represents a Programmable Command Format (PCF) message.
{
public:

	explicit PCF(bool zos = false);
		/// Creates an empty PCF message
		/// Use this constructor to get a PCF message from a queue.

	explicit PCF(int cmd, bool zos = false);
		/// Creates an empty PCF message for a command
		/// Use this constructor to put a PFC on a queue.

	virtual ~PCF();
		/// Destructor.

	void addParameter(MQLONG parameter, const std::string& value);
		/// Add a string parameter.

	void addParameter(MQLONG parameter, MQLONG value);
		/// Add a numeric parameter.

	void addParameterList(MQLONG parameter, MQLONG *values);
		/// Add a numeric list parameter.

	void addFilter(MQLONG parameter, MQLONG op, const std::string& value);
		/// Add a filter with a string value.

	void addFilter(MQLONG parameter, MQLONG op, MQLONG value);
		/// Add a filter with a numeric value.

	int getCommand() const;
		/// Returns the command.

	int getCompletionCode() const;
		/// Returns the completion code.

	int getReasonCode() const;
		/// Returns the reason code.

	bool isExtendedResponse() const;
		/// Returns true when this is an extended response.

	bool isNumber(MQLONG parameter) const;
		/// Returns true when the value of the parameter is a numeric value.

	bool isString(MQLONG parameter) const;
		/// Returns true when the value of the parameter is a string value.

	bool isLast() const;
		/// Returns true when this PCF message is the last of a response.

	Poco::DateTime getParameterDate(MQLONG dateParameter, MQLONG timeParameter) const;
		/// Combines a date and time parameter and returns it as a DateTime object
		/// When the date parameter doesn't exist, the current date is returned.

	MQLONG getParameterNum(MQLONG parameter) const;
		/// Returns the numeric value of a parameter.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a numeric value.

	std::string getParameterString(MQLONG parameter) const;
		/// Returns the string value of a parameter.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a string value.

	std::vector<std::string> getParameterStringList(MQLONG parameter) const;
		/// Returns a vector of strings of a parameter.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a string list.

	bool hasParameter(MQLONG parameter) const;
		/// Returns true when the parameter is found in the PCF message.

	std::vector<MQLONG> getParameters() const;
		/// Returns a vector with all parameter ids.

	void init();
		/// Initializes the internal PCF structures. Call this when you retrieved
		/// a PFC message from a queue and before you start retrieving information
		/// from it.

	MQLONG optParameterNum(MQLONG parameter, MQLONG def = 0) const;
		/// Returns the numeric value of a parameter.
		/// When the parameter isn't found or doesn't contain a numeric value,
		/// def will be returned.

	std::string optParameterString(MQLONG parameter, const std::string& def = "") const;
		/// Returns the string value of a parameter.
		/// When the parameter doesn't exist or is not a string, an empty
		/// string is returned.

	typedef Poco::SharedPtr<PCF> Ptr;

	typedef std::vector<Ptr> Vector;

private:

	std::map<MQLONG, int> _pointers;


	bool _zos;


	PCF(const PCF& pcf);


	friend class CommandServer;
};


inline int PCF::getCommand() const 
{
	MQCFH* header = (MQCFH*)(MQBYTE*) &buffer()[0];
	return header->Command; 
}

inline int PCF::getCompletionCode() const 
{ 
	MQCFH* header = (MQCFH*)(MQBYTE*) &buffer()[0];
	return header->CompCode; 
}

inline int PCF::getReasonCode() const 
{ 
	MQCFH* header = (MQCFH*)(MQBYTE*) &buffer()[0];
	return header->Reason; 
}

inline bool PCF::isExtendedResponse() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) &buffer()[0];
	return header->Type == MQCFT_XR_SUMMARY; 
}


inline bool PCF::hasParameter(MQLONG parameter) const
{
	return _pointers.find(parameter) != _pointers.end();
}

inline bool PCF::isLast() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) &buffer()[0];
	return header->Control == MQCFC_LAST;
}

inline bool PCF::isNumber(MQLONG parameter) const
{
	std::map<MQLONG, int>::const_iterator it = _pointers.find(parameter);
	if ( it != _pointers.end() )
	{
		MQLONG *pcfType = (MQLONG*) &buffer()[it->second];
		return *pcfType == MQCFT_INTEGER;
	}
	return false;
}

inline bool PCF::isString(MQLONG parameter) const
{
	std::map<MQLONG, int>::const_iterator it = _pointers.find(parameter);
	if ( it != _pointers.end() )
	{
		MQLONG *pcfType = (MQLONG*) &buffer()[it->second];
		return *pcfType == MQCFT_STRING || *pcfType == MQCFT_BYTE_STRING;
	}
	return false;
}

} // MQ namespace

#endif  //_MQ_PCF_h
