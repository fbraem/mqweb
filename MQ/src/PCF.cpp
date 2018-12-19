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
#include <cmqc.h>

#include "MQ/PCF.h"

namespace MQ
{

PCF::Ptr PCF::create(Message::Ptr message, bool zos)
{
	return new PCF(message, zos);
}

PCF::PCF(Message::Ptr message, bool zos)
	: _message(message), _zos(zos), _parameters(_message->buffer())
{
	MQCFH* header = (MQCFH*)(MQBYTE*) _message->buffer().data();
	_parameters.parse(header->ParameterCount, MQCFH_STRUC_LENGTH);
}

PCF::PCF(int cmd, bool zos)
	: _zos(zos), _message(new Message(MQCFH_STRUC_LENGTH)), _parameters(_message->buffer())
{
	//_message = new Message(MQCFH_STRUC_LENGTH);
	_message->setFormat(MQFMT_ADMIN);
	_message->setType(MQMT_REQUEST);
	_message->setPersistence(MQPER_NOT_PERSISTENT);
	MQCFH* header = (MQCFH*)(MQBYTE*) _message->buffer().data();
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


void PCF::addParameter(MQLONG parameter, const std::string& value)
{
	_parameters.add(parameter, value);
	incrementParameterCount();
}


void PCF::addParameter(MQLONG parameter, MQLONG value)
{
	_parameters.add(parameter, value);
	incrementParameterCount();
}

void PCF::addParameter(MQLONG parameter, Buffer::Ptr value)
{
	_parameters.add(parameter, value);
	incrementParameterCount();
}

void PCF::addParameterList(MQLONG parameter, MQLONG *values, unsigned int count)
{
	_parameters.addList(parameter, values, count);
	incrementParameterCount();
}


void PCF::addFilter(MQLONG parameter, MQLONG op, const std::string& value)
{
	_parameters.addFilter(parameter, op, value);
	incrementParameterCount();
}


void PCF::addFilter(MQLONG parameter, MQLONG op, MQLONG value)
{
	_parameters.addFilter(parameter, op, value);
	incrementParameterCount();
}

} // Namespace MQ
