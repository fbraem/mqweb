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
#include <string.h> // For memcpy

#include "Poco/String.h"
#include "Poco/DateTimeParser.h"

#include "MQ/PCFParameters.h"

namespace MQ
{

PCFParameters::PCFParameters(Buffer& buffer) : _buffer(buffer)
{
}

PCFParameters::PCFParameters(const PCFParameters& copy) : _buffer(copy._buffer), _pointers(copy._pointers)
{
}

PCFParameters::~PCFParameters()
{
}

void PCFParameters::add(MQLONG parameter, const std::string& value)
{
	MQLONG structLength = ((MQCFST_STRUC_LENGTH_FIXED + (MQLONG) value.length()) / 4 + 1) * 4;
	size_t pos = _buffer.size();
	_pointers.insert(std::make_pair(parameter, pos));
	_buffer.resize(_buffer.size() + structLength);
	MQCFST* pcfParam = (MQCFST*) _buffer.data(pos);
	pcfParam->Type           = MQCFT_STRING;
	pcfParam->StrucLength    = structLength;
	pcfParam->Parameter      = parameter;
	pcfParam->CodedCharSetId = MQCCSI_DEFAULT;
	pcfParam->StringLength   = (MQLONG) value.length();
	memcpy(pcfParam->String, value.c_str(), pcfParam->StringLength);
}


void PCFParameters::add(MQLONG parameter, MQLONG value)
{
	size_t pos =  _buffer.size();
	_pointers.insert(std::make_pair(parameter, pos)); 
	_buffer.resize(_buffer.size() + MQCFIN_STRUC_LENGTH);
	MQCFIN* pcfParam = (MQCFIN*) _buffer.data(pos);
	pcfParam->Type        = MQCFT_INTEGER;
	pcfParam->StrucLength = MQCFIN_STRUC_LENGTH;
	pcfParam->Parameter   = parameter;
	pcfParam->Value       = value;
}

void PCFParameters::add(MQLONG parameter, Buffer::Ptr value)
{
	size_t pos = _buffer.size();
	_pointers.insert(std::make_pair(parameter, pos));
	MQLONG structLength = ((MQCFST_STRUC_LENGTH_FIXED + (MQLONG) value->size()) / 4 + 1) * 4;
	_buffer.resize(_buffer.size() + structLength);
	MQCFBS* pcfParam = (MQCFBS*) _buffer.data(pos);
	pcfParam->Type           = MQCFT_BYTE_STRING;
	pcfParam->StrucLength    = structLength;
	pcfParam->Parameter      = parameter;
	pcfParam->StringLength   = (MQLONG) value->size();
	memcpy(pcfParam->String, value->data(), pcfParam->StringLength);
}

void PCFParameters::addList(MQLONG parameter, MQLONG *values, unsigned int count)
{
	size_t pos = _buffer.size();
	int strucLength = MQCFIL_STRUC_LENGTH_FIXED + count * sizeof(MQLONG);
	_pointers.insert(std::make_pair(parameter, pos));
	_buffer.resize(_buffer.size() + strucLength);
	MQCFIL *pcfIntegerList = (MQCFIL *) _buffer.data(pos);
	pcfIntegerList->Count       = count;
	pcfIntegerList->Type        = MQCFT_INTEGER_LIST;
	pcfIntegerList->StrucLength = strucLength;
	pcfIntegerList->Parameter   = parameter;
	for(int i = 0; i < count; ++i) pcfIntegerList->Values[i] = values[i];
}


void PCFParameters::addFilter(MQLONG parameter, MQLONG op, const std::string& value)
{
	size_t pos = _buffer.size();
	MQLONG strucLength = ((MQCFSF_STRUC_LENGTH_FIXED + (MQLONG) value.length()) / 4 + 1) * 4;
	_pointers.insert(std::make_pair(parameter, pos));
	_buffer.resize(_buffer.size() + strucLength);
	MQCFSF* pcfFilter = (MQCFSF*) _buffer.data(pos);
	pcfFilter->Type              = MQCFT_STRING_FILTER;
	pcfFilter->StrucLength       = strucLength;
	pcfFilter->Parameter         = parameter;
	pcfFilter->Operator          = op;
	pcfFilter->CodedCharSetId    = MQCCSI_DEFAULT;
	pcfFilter->FilterValueLength = (MQLONG) value.length();
	memcpy(pcfFilter->FilterValue, value.c_str(), pcfFilter->FilterValueLength);
}

void PCFParameters::addFilter(MQLONG parameter, MQLONG op, MQLONG value)
{
	size_t pos = _buffer.size();
	_pointers.insert(std::make_pair(parameter, pos));
	_buffer.resize(_buffer.size() + MQCFIF_STRUC_LENGTH);
	MQCFIF* pcfFilter = (MQCFIF*) _buffer.data(pos);
	pcfFilter->Type              = MQCFT_INTEGER_FILTER;
	pcfFilter->StrucLength       = MQCFIF_STRUC_LENGTH;
	pcfFilter->Parameter         = parameter;
	pcfFilter->Operator          = op;
	pcfFilter->FilterValue       = value;
}

PCFParameters PCFParameters::getGroup(MQLONG parameter, size_t pos) const
{
	size_t realPos = 0;
	std::pair<ParameterPositionMap::const_iterator, ParameterPositionMap::const_iterator> range = _pointers.equal_range(parameter);
	size_t i = 0;
	for(ParameterPositionMap::const_iterator it = range.first; it != range.second; ++it, ++i)
	{
		if ( i == pos ) 
		{
			realPos = it->second;
			break;
		}
	}
	if ( realPos == 0 )
	{
		throw Poco::NotFoundException(parameter);
	}

	MQLONG *pcfType = (MQLONG*) _buffer.data(realPos);
	if ( *pcfType == MQCFT_GROUP )
	{
		MQCFGR* pcfParam = (MQCFGR*) _buffer.data(realPos);
		PCFParameters group(_buffer);
		group.parse(pcfParam->ParameterCount, realPos + MQCFGR_STRUC_LENGTH);
		return group;
	}

	throw Poco::BadCastException(parameter);
}

std::string PCFParameters::getString(MQLONG parameter) const
{
	ParameterPositionMap::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) _buffer.data(it->second);
	if ( *pcfType == MQCFT_STRING )
	{
		MQCFST* pcfParam = (MQCFST*) _buffer.data(it->second);
		std::string result(pcfParam->String, pcfParam->StringLength);

		MQLONG length = pcfParam->StringLength -1;
		for(; length > 0 && (result[length] == '\0' || result[length] == ' ');           \
			length--);
		result.resize(length + 1);
		return result;
	}
	else if ( *pcfType == MQCFT_BYTE_STRING )
	{
		MQCFBS* pcfParam = (MQCFBS*) _buffer.data(it->second);
		return Buffer((const MQBYTE*) pcfParam->String, pcfParam->StringLength).toHex();
	}

	throw Poco::BadCastException(parameter);
}

Buffer::Ptr PCFParameters::getByteString(MQLONG parameter) const
{
	ParameterPositionMap::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) _buffer.data(it->second);
	if ( *pcfType == MQCFT_BYTE_STRING )
	{
		MQCFBS* pcfParam = (MQCFBS*) _buffer.data(it->second);
		return new Buffer(pcfParam->String, pcfParam->StringLength);
	}

	throw Poco::BadCastException(parameter);
}

std::string PCFParameters::optString(MQLONG parameter, const std::string& def) const
{
	std::string result = def;

	try
	{
		result = getString(parameter);
	}
	catch(...)
	{
	}

	return result;
}

Poco::DateTime PCFParameters::getDate(MQLONG dateParameter, MQLONG timeParameter) const
{
	std::string dateValue = getString(dateParameter);
	Poco::trimRightInPlace(dateValue);
	std::string timeValue = getString(timeParameter);
	Poco::trimRightInPlace(timeValue);
	dateValue += timeValue;
	if ( ! dateValue.empty() )
	{
		Poco::DateTime dateTime;
		int timeZone;
		Poco::DateTimeParser::parse("%Y%n%e%H%M%S", dateValue, dateTime, timeZone);
		return dateTime;
	}
	return Poco::DateTime();
}


MQLONG PCFParameters::getNumber(MQLONG parameter) const
{
	ParameterPositionMap::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) _buffer.data(it->second);
	if ( *pcfType == MQCFT_INTEGER )
	{
		MQCFIN* pcfParam = (MQCFIN*) _buffer.data(it->second);
		return pcfParam->Value;
	}

	throw Poco::BadCastException(parameter);
}

std::vector<MQLONG> PCFParameters::getNumberList(MQLONG parameter) const
{
	ParameterPositionMap::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) _buffer.data(it->second);
	if ( *pcfType == MQCFT_INTEGER_LIST )
	{
		std::vector<MQLONG> list;
		MQCFIL* pcfParam = (MQCFIL*) _buffer.data(it->second);
		for(int i = 0; i < pcfParam->Count; ++i)
		{
			list.push_back(pcfParam->Values[i]);
		}
		return list;
	}

	throw Poco::BadCastException(parameter);
}

std::vector<MQINT64> PCFParameters::getNumber64List(MQLONG parameter) const
{
	ParameterPositionMap::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) _buffer.data(it->second);
	if ( *pcfType == MQCFT_INTEGER64_LIST )
	{
		std::vector<MQINT64> list;
		MQCFIL64* pcfParam = (MQCFIL64*) _buffer.data(it->second);
		for(int i = 0; i < pcfParam->Count; ++i)
		{
			list.push_back(pcfParam->Values[i]);
		}
		return list;
	}

	throw Poco::BadCastException(parameter);
}

std::vector<std::string> PCFParameters::getStringList(MQLONG parameter) const
{
	ParameterPositionMap::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) _buffer.data(it->second);
	if ( *pcfType == MQCFT_STRING_LIST )
	{
		std::vector<std::string> list;
		MQCFSL* pcfParam = (MQCFSL*) _buffer.data(it->second);
		for(int i = 0; i < pcfParam->Count; ++i)
		{
			std::string result(pcfParam->Strings, i * pcfParam->StringLength, pcfParam->StringLength);
			Poco::trimRightInPlace(result);
			list.push_back(result);
		}
		return list;
	}

	throw Poco::BadCastException(parameter);
}

MQLONG PCFParameters::optNumber(MQLONG parameter, MQLONG def) const
{
	MQLONG result = def;
	try
	{
		result = getNumber(parameter);
	}
	catch(...)
	{
	}
	return result;
}

std::vector<MQLONG> PCFParameters::getIds() const
{
	std::vector<MQLONG> parameters;
	for(ParameterPositionMap::const_iterator it = _pointers.begin(); it != _pointers.end(); it++)
	{
		MQLONG *pcfType = (MQLONG*) _buffer.data(it->second);
		parameters.push_back(pcfType[2]);
	}
	return parameters;
}

void PCFParameters::parse(MQLONG parameterCount, size_t pos)
{
	for(MQLONG i = 0; i < parameterCount; ++i)
	{
		MQLONG *pcfType = (MQLONG*) _buffer.data(pos);
		_pointers.insert(std::make_pair(pcfType[2], pos));
		setNextPos(pcfType, pos);
	}
}

void PCFParameters::setNextPos(MQLONG* pcfType, size_t& pos)
{
	if (*pcfType == MQCFT_GROUP)
	{
		pos += pcfType[1];
		MQCFGR* pcfGroup = (MQCFGR*) pcfType;
		for(MQLONG i = 0; i < pcfGroup->ParameterCount; ++i)
		{
			setNextPos((MQLONG*) _buffer.data(pos), pos);
		}
	}
	else
	{
		pos += pcfType[1];
	}
}

} // namespace MQ
