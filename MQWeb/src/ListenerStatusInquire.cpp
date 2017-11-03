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
#include "MQ/Web/ListenerStatusInquire.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {

ListenerStatusInquire::ListenerStatusInquire(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: PCFCommand(commandServer, MQCMD_INQUIRE_LISTENER_STATUS, "ListenerStatus", input), _excludeSystem(false)
{
	// Required parameters
	addParameter<std::string>(MQCACH_LISTENER_NAME, "ListenerName");

	// Optional parameters
	addIntegerFilter();
	addAttributeList(MQIACF_LISTENER_STATUS_ATTRS, "ListenerStatusAttrs");
	addStringFilter();

	_excludeSystem = input->optValue("ExcludeSystem", false);
}

ListenerStatusInquire::~ListenerStatusInquire()
{
}


Poco::JSON::Array::Ptr ListenerStatusInquire::execute()
{
	PCFCommand::execute();

	Poco::JSON::Array::Ptr statuses = new Poco::JSON::Array();

	if (hasResponse()) 
	{
		PCF::Vector::const_iterator it = begin();
		if ( (*it)->getReasonCode() != MQRC_NONE 
			&& (*it)->getReasonCode() != MQRC_UNKNOWN_OBJECT_NAME)
		{
			throw MQException("PCF", getCommandString((*it)->getCommand()), (*it)->getCompletionCode(), (*it)->getReasonCode());
		}

		for(; it != end(); it++)
		{
			if ( (*it)->isExtendedResponse() ) // Skip extended response
				continue;

			if ( (*it)->getReasonCode() != MQRC_NONE )
				continue;

			std::string listenerName = (*it)->getParameterString(MQCACH_LISTENER_NAME);
			if ( _excludeSystem
				&& listenerName.compare(0, 7, "SYSTEM.") == 0 )
			{
				continue;
			}

			statuses->add(createJSON(**it));
		}
	}

	return statuses;
}

}} //  Namespace MQ::Web
