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
#include <string.h> // For memcpy
#include <cmqc.h>

#include "Poco/DateTimeParser.h"
#include "MQ/PCF.h"

namespace MQ
{

PCF::PCF(bool zos)
	: Message()
	, _zos(zos)
{
}


PCF::PCF(int cmd, bool zos)
	: Message()
	, _zos(zos)
{
	setFormat(MQFMT_ADMIN);
	setType(MQMT_REQUEST);
	setPersistence(MQPER_NOT_PERSISTENT);
	buffer().resize(MQCFH_STRUC_LENGTH);
	MQCFH* header = (MQCFH*)(MQBYTE*) &buffer()[0];
	header->StrucLength    = MQCFH_STRUC_LENGTH;
	if ( _zos )
	{
		header->Type    = MQCFT_COMMAND_XR;
		header->Version = MQCFH_VERSION_3;
	}
	else
	{
		header->Type       = (MQLONG) MQCFT_COMMAND;
		header->Version    = MQCFH_VERSION_1;
	}
	header->Command        = cmd;
	header->MsgSeqNumber   = MQCFC_LAST;
	header->Control        = MQCFC_LAST;
	header->ParameterCount = 0;
}


PCF::~PCF()
{
}


void PCF::init()
{
	MQCFH* header = (MQCFH*)(MQBYTE*) &(buffer()[0]);
	int pos = MQCFH_STRUC_LENGTH;
	for(int i = 0; i < header->ParameterCount; i++)
	{
		MQLONG *pcfType = (MQLONG*) &buffer()[pos];
		_pointers[pcfType[2]] = pos;
		pos += pcfType[1];
	}
}


std::string PCF::getParameterString(MQLONG parameter) const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) &(buffer()[0]);
	std::map<MQLONG, int>::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) &buffer()[it->second];
	if ( *pcfType == MQCFT_STRING )
	{
		MQCFST* pcfParam = (MQCFST*) &buffer()[it->second];
		std::string result(pcfParam->String, pcfParam->StringLength);
		return Poco::trimRightInPlace(result);
	}
	else if ( *pcfType == MQCFT_BYTE_STRING )
	{
		MQCFBS* pcfParam = (MQCFBS*) &buffer()[it->second];
		return Message::getBufferAsHex(pcfParam->String, pcfParam->StringLength);
	}

	throw Poco::BadCastException(parameter);
}

BufferPtr PCF::getParameterByteString(MQLONG parameter) const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) &(buffer()[0]);
	std::map<MQLONG, int>::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) &buffer()[it->second];
	if ( *pcfType == MQCFT_BYTE_STRING )
	{
		MQCFBS* pcfParam = (MQCFBS*) &buffer()[it->second];
		return new Buffer(pcfParam->String, pcfParam->StringLength);
	}

	throw Poco::BadCastException(parameter);
}

std::string PCF::optParameterString(MQLONG parameter, const std::string& def) const
{
	std::string result = def;

	try
	{
		result = getParameterString(parameter);
	}
	catch(...)
	{
	}

	return result;
}

Poco::DateTime PCF::getParameterDate(MQLONG dateParameter, MQLONG timeParameter) const
{
	std::string dateValue = getParameterString(dateParameter);
	Poco::trimRightInPlace(dateValue);
	std::string timeValue = getParameterString(timeParameter);
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


MQLONG PCF::getParameterNum(MQLONG parameter) const
{
	std::map<MQLONG, int>::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) &buffer()[it->second];
	if ( *pcfType == MQCFT_INTEGER )
	{
		MQCFIN* pcfParam = (MQCFIN*) &buffer()[it->second];
		return pcfParam->Value;
	}

	throw Poco::BadCastException(parameter);
}

std::vector<MQLONG> PCF::getParameterNumList(MQLONG parameter) const
{
	std::map<MQLONG, int>::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) &buffer()[it->second];
	if ( *pcfType == MQCFT_INTEGER_LIST )
	{
		std::vector<MQLONG> list;
		MQCFIL* pcfParam = (MQCFIL*) &buffer()[it->second];
		for(int i = 0; i < pcfParam->Count; ++i)
		{
			list.push_back(pcfParam->Values[i]);
		}
		return list;
	}

	throw Poco::BadCastException(parameter);
}

std::vector<std::string> PCF::getParameterStringList(MQLONG parameter) const
{
	std::map<MQLONG, int>::const_iterator it = _pointers.find(parameter);
	if ( it == _pointers.end() )
		throw Poco::NotFoundException(parameter);

	MQLONG *pcfType = (MQLONG*) &buffer()[it->second];
	if ( *pcfType == MQCFT_STRING_LIST )
	{
		std::vector<std::string> list;
		MQCFSL* pcfParam = (MQCFSL*) &buffer()[it->second];
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

MQLONG PCF::optParameterNum(MQLONG parameter, MQLONG def) const
{
	MQLONG result = def;
	try
	{
		result = getParameterNum(parameter);
	}
	catch(...)
	{
	}
	return result;
}


void PCF::addParameter(MQLONG parameter, const std::string& value)
{
	MQLONG structLength = ((MQCFST_STRUC_LENGTH_FIXED + value.length()) / 4 + 1) * 4;
	_pointers[parameter] = buffer().size();
	buffer().resize(buffer().size() + structLength);
	MQCFST* pcfParam = (MQCFST*) &buffer()[_pointers[parameter]];
	pcfParam->Type           = MQCFT_STRING;
	pcfParam->StrucLength    = structLength;
	pcfParam->Parameter      = parameter;
	pcfParam->CodedCharSetId = MQCCSI_DEFAULT;
	pcfParam->StringLength   = value.length();
	memcpy(pcfParam->String, value.c_str(), pcfParam->StringLength);
	MQCFH* header = (MQCFH*) (MQBYTE*) &buffer()[0];
	header->ParameterCount++;
}


void PCF::addParameter(MQLONG parameter, MQLONG value)
{
	_pointers[parameter] = buffer().size();
	buffer().resize(buffer().size() + MQCFIN_STRUC_LENGTH);
	MQCFIN* pcfParam = (MQCFIN*) &buffer()[_pointers[parameter]];
	pcfParam->Type        = MQCFT_INTEGER;
	pcfParam->StrucLength = MQCFIN_STRUC_LENGTH;
	pcfParam->Parameter   = parameter;
	pcfParam->Value       = value;
	MQCFH* header = (MQCFH*) (MQBYTE*) &buffer()[0];
	header->ParameterCount++;
}

void PCF::addParameter(MQLONG parameter, BufferPtr value)
{
	MQLONG structLength = ((MQCFST_STRUC_LENGTH_FIXED + value->sizeBytes()) / 4 + 1) * 4;
	_pointers[parameter] = buffer().size();
	buffer().resize(buffer().size() + structLength);
	MQCFBS* pcfParam = (MQCFBS*) &buffer()[_pointers[parameter]];
	pcfParam->Type           = MQCFT_BYTE_STRING;
	pcfParam->StrucLength    = structLength;
	pcfParam->Parameter      = parameter;
	pcfParam->StringLength   = value->sizeBytes();
	memcpy(pcfParam->String, value->begin(), pcfParam->StringLength);
	MQCFH* header = (MQCFH*) (MQBYTE*) &buffer()[0];
	header->ParameterCount++;
}

void PCF::addParameterList(MQLONG parameter, MQLONG *values)
{
	int count = (sizeof values / sizeof values[0]);
	int strucLength = MQCFIL_STRUC_LENGTH_FIXED + count * 4;
	_pointers[parameter] = buffer().size();
	buffer().resize(buffer().size() + strucLength);
	MQCFIL *pcfIntegerList = (MQCFIL *) &buffer()[_pointers[parameter]];
	pcfIntegerList->Count       = count;
	pcfIntegerList->Type        = MQCFT_INTEGER_LIST;
	pcfIntegerList->StrucLength = strucLength;
	pcfIntegerList->Parameter   = parameter;
	pcfIntegerList->Values[0]   = *values;
	MQCFH* header = (MQCFH*) (MQBYTE*) &buffer()[0];
	header->ParameterCount++;
}


void PCF::addFilter(MQLONG parameter, MQLONG op, const std::string& value)
{
	MQLONG strucLength = ((MQCFSF_STRUC_LENGTH_FIXED + value.length()) / 4 + 1) * 4;
	_pointers[parameter] = buffer().size();
	buffer().resize(buffer().size() + strucLength);
	MQCFSF* pcfFilter = (MQCFSF*) &buffer()[_pointers[parameter]];
	pcfFilter->Type              = MQCFT_STRING_FILTER;
	pcfFilter->StrucLength       = strucLength;
	pcfFilter->Parameter         = parameter;
	pcfFilter->Operator          = op;
	pcfFilter->CodedCharSetId    = MQCCSI_DEFAULT;
	pcfFilter->FilterValueLength = value.length();
	memcpy(pcfFilter->FilterValue, value.c_str(), pcfFilter->FilterValueLength);
	MQCFH* header = (MQCFH*) (MQBYTE*) &buffer()[0];
	header->ParameterCount++;
}


void PCF::addFilter(MQLONG parameter, MQLONG op, MQLONG value)
{
	_pointers[parameter] = buffer().size();
	buffer().resize(buffer().size() + MQCFIF_STRUC_LENGTH);
	MQCFIF* pcfFilter = (MQCFIF*) &buffer()[_pointers[parameter]];
	pcfFilter->Type              = MQCFT_INTEGER_FILTER;
	pcfFilter->StrucLength       = MQCFIF_STRUC_LENGTH;
	pcfFilter->Parameter         = parameter;
	pcfFilter->Operator          = op;
	pcfFilter->FilterValue       = value;
	MQCFH* header = (MQCFH*) (MQBYTE*) &buffer()[0];
	header->ParameterCount++;
}


std::vector<MQLONG> PCF::getParameters() const
{
	std::vector<MQLONG> parameters;
	for(std::map<MQLONG, int>::const_iterator it = _pointers.begin(); it != _pointers.end(); it++)
	{
		MQLONG *pcfType = (MQLONG*) &buffer()[it->second];
		parameters.push_back(pcfType[2]);
	}
	return parameters;
}

} // Namespace MQ
