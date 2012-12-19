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

namespace MQ
{

class PCF : public Message
{
public:

explicit PCF(bool zos = false);


explicit PCF(int cmd, bool zos = false);


virtual ~PCF();


void addParameter(MQLONG parameter, const std::string& value);


void addParameter(MQLONG parameter, MQLONG value);


void addParameterList(MQLONG parameter, MQLONG *values);


void addFilter(MQLONG parameter, MQLONG op, const std::string& value);


void addFilter(MQLONG parameter, MQLONG op, MQLONG value);


int getCommand() const;


int getCompletionCode() const;


int getReasonCode() const;


bool isExtendedResponse() const;


bool isNumber(MQLONG parameter) const;


bool isString(MQLONG parameter) const;


bool isLast() const;


std::string getParameterString(MQLONG parameter) const;


Poco::DateTime getParameterDate(MQLONG dateParameter, MQLONG timeParameter) const;


MQLONG getParameterNum(MQLONG parameter) const;


bool hasParameter(MQLONG parameter) const;


std::vector<MQLONG> getParameters() const;


typedef Poco::SharedPtr<PCF> Ptr;


typedef std::vector<Ptr> Vector;

private:

std::map<MQLONG, int> _pointers;


bool _zos;


PCF(const PCF& pcf);


void init();


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
