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
#include "MQ/Web/QueueRemove.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {

QueueRemove::QueueRemove(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: PCFCommand(commandServer, MQCMD_DELETE_Q, "Queue", input)
{
	// Required Parameters
	addParameter<std::string>(MQCA_Q_NAME, "QName");

	// Optional Parameters
	std::string authrec = input->optValue<std::string>("Authrec", "No");
	if ( Poco::icompare(authrec, "Yes") == 0 )
	{
		pcf()->addParameter(MQIACF_REMOVE_AUTHREC, MQRAR_YES);
	}
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");

	std::string purge = input->optValue<std::string>("Purge", "No");
	if ( Poco::icompare(authrec, "Yes") == 0 )
	{
		pcf()->addParameter(MQIACF_PURGE, MQPO_YES);
	}

	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	addParameterNumFromString(MQIA_Q_TYPE, "QType");
}

QueueRemove::~QueueRemove()
{
}

Poco::JSON::Array::Ptr QueueRemove::execute()
{
	PCFCommand::execute();

	for(PCF::Vector::const_iterator it = begin(); it != end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE )
		{
			throw MQException("PCF", getCommandString((*it)->getCommand()), (*it)->getCompletionCode(), (*it)->getReasonCode());
		}
	}
	return new Poco::JSON::Array();
}

}} //  Namespace MQ::Web
