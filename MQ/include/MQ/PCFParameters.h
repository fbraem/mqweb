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
#ifndef _MQ_PCFParameters_h
#define _MQ_PCFParameters_h

#include <map>
#include <vector>

#include <cmqc.h>
#include <cmqcfc.h> /* PCF  */

#include "Poco/DateTime.h"

#include "MQ/Buffer.h"

typedef std::multimap<MQLONG, size_t> ParameterPositionMap;

namespace MQ {

class PCFParameters {
	/// Helper class for a PCF message and a PCF group structure.
	/// This class is not the owner of the passed buffer, so make
	/// sure that the lifetime of an instance isn't longer then
	/// the PCF message it belongs to.
public:
	PCFParameters(Buffer& buffer);
		/// Constructor

	PCFParameters(const PCFParameters& copy);
		/// Copy constructor

	virtual ~PCFParameters();
		/// Destructor

	void add(MQLONG parameter, const std::string& value);
		/// Add a string parameter.

	void add(MQLONG parameter, MQLONG value);
		/// Add a numeric parameter.

	void add(MQLONG parameter, Buffer::Ptr buffer);
		/// Add a byte string parameter.

	void addList(MQLONG parameter, MQLONG *values, unsigned int count);
		/// Add a numeric list parameter.

	void addFilter(MQLONG parameter, MQLONG op, const std::string& value);
		/// Add a filter with a string value.

	void addFilter(MQLONG parameter, MQLONG op, MQLONG value);
		/// Add a filter with a numeric value.

	void addStringList(MQLONG parameter, const std::vector<std::string>& list);
		/// Adds a String list

	bool isByteString(MQLONG parameter) const;
		/// Returns true when the value of the parameter is a byte string.

	bool isExtendedResponse() const;
		/// Returns true when this is an extended response.

	bool isGroup(MQLONG parameter) const;
		/// Returns true when the value of the parameter contains grouped parameters.

	bool isNumber(MQLONG parameter) const;
		/// Returns true when the value of the parameter is a numeric value.

	bool isNumberList(MQLONG parameter) const;
		/// Returns true when the value of the parameter is a numeric list.

	bool isNumber64List(MQLONG parameter) const;
		/// Returns true when the value of the parameter is a numeric list with 64bit integers.

	bool isString(MQLONG parameter) const;
		/// Returns true when the value of the parameter is a string value.
		/// Note: a byte string will also return true! When getParameterString is called
		/// a byte string will be returned as hex. Use isByteString to check if a
		/// value contains byte string

	bool isStringList(MQLONG parameter) const;
		/// Returns true when the value of the parameter is a string list value.

	Poco::DateTime getDate(MQLONG dateParameter, MQLONG timeParameter) const;
		/// Combines a date and time parameter and returns it as a DateTime object
		/// When the date parameter doesn't exist, the current date is returned.

	MQLONG getNumber(MQLONG parameter) const;
		/// Returns the numeric value of a parameter.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a numeric value.

	PCFParameters getGroup(MQLONG parameter, size_t pos = 0) const;
		/// Returns a group of PCF parameters. Multiple groups with the same parameter id are possible.

	size_t getGroupCount(MQLONG parameter) const;
		/// Returns the number of groups of PCF parameters with the given parameter id.

	std::vector<MQLONG> getNumberList(MQLONG parameter) const;
		/// Returns a numeric list.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a numeric list value.

	std::vector<MQINT64> getNumber64List(MQLONG parameter) const;
		/// Returns a numeric list.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a numeric list value.

	std::string getString(MQLONG parameter) const;
		/// Returns the string value of a parameter. A byte string is converted to a hex value.
		/// If you need the real byte string, use getParameterByteString.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a string or byte string value.

	Buffer::Ptr getByteString(MQLONG parameter) const;
		/// Returns the byte string value as a buffer.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a byte string value.

	std::vector<std::string> getStringList(MQLONG parameter) const;
		/// Returns a vector of strings of a parameter.
		/// Poco::NotFoundException will be thrown when the parameter isn't found.
		/// Poco::BadCastException will be thrown when the parameter doesn't contain a string list.

	bool has(MQLONG parameter) const;
		/// Returns true when the parameter is found in the PCF message.

	std::vector<MQLONG> getIds() const;
		/// Returns a vector with all parameter ids.

	MQLONG optNumber(MQLONG parameter, MQLONG def = 0) const;
		/// Returns the numeric value of a parameter.
		/// When the parameter isn't found or doesn't contain a numeric value,
		/// def will be returned.

	std::string optString(MQLONG parameter, const std::string& def = "") const;
		/// Returns the string value of a parameter.
		/// When the parameter doesn't exist or is not a string, an empty
		/// string is returned.

	void parse(MQLONG parameterCount, size_t pos = 0);
		/// Parse the current buffer for parameters.

private:
	Buffer& _buffer;

	ParameterPositionMap _pointers;
		/// A multimap because group PCF parameters can have multiple
		/// values with the same id.

	bool isType(MQLONG parameter, MQLONG type) const;
		/// Returns true when the parameter is of given type.

	void setNextPos(MQLONG* pcfType, size_t& pos);
		/// Helper function to get the position of the next parameter.
};

inline size_t PCFParameters::getGroupCount(MQLONG parameter) const
{
	return _pointers.count(parameter);
}

inline bool PCFParameters::isByteString(MQLONG parameter) const
{
	return isType(parameter, MQCFT_BYTE_STRING);
}

inline bool PCFParameters::has(MQLONG parameter) const
{
	return _pointers.find(parameter) != _pointers.end();
}

inline bool PCFParameters::isGroup(MQLONG parameter) const
{
	return isType(parameter, MQCFT_GROUP);
}

inline bool PCFParameters::isNumber(MQLONG parameter) const
{
	return isType(parameter, MQCFT_INTEGER);
}

inline bool PCFParameters::isNumberList(MQLONG parameter) const
{
	return isType(parameter, MQCFT_INTEGER_LIST);
}

inline bool PCFParameters::isNumber64List(MQLONG parameter) const
{
	return isType(parameter, MQCFT_INTEGER64_LIST);
}

inline bool PCFParameters::isString(MQLONG parameter) const
{
	return isType(parameter, MQCFT_STRING) || isType(parameter, MQCFT_BYTE_STRING);
}

inline bool PCFParameters::isStringList(MQLONG parameter) const
{
	return isType(parameter, MQCFT_STRING_LIST);
}

inline bool PCFParameters::isType(MQLONG parameter, MQLONG type) const
{
	std::multimap<MQLONG, size_t>::const_iterator it = _pointers.find(parameter);
	if ( it != _pointers.end() )
	{
		MQLONG *pcfType = (MQLONG*) _buffer.data(it->second);
		return *pcfType == type;
	}
	return false;
}

} // MQ Namespace

#endif // _MQ_PCFParameters_h
