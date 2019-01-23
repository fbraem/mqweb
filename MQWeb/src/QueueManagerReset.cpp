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
#include "MQ/Web/QueueManagerReset.h"
#include "MQ/Web/Dictionary.h"

namespace MQ {
namespace Web {


QueueManagerReset::QueueManagerReset(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: PCFCommand(commandServer, MQCMD_RESET_Q_MGR, "QueueManager", input)
{
	static TextMap actionMap = TextMapInitializer
		(MQACT_ADVANCE_LOG, "AdvanceLog")
		(MQACT_COLLECT_STATISTICS, "Statistics")
		(MQACT_PUBSUB, "PubSub")
	;
	std::string action = input->get("Action");
	for (TextMap::const_iterator it = actionMap.begin(); it != actionMap.end(); ++it)
	{
		if (it->second.compare(action) == 0) {
			pcf()->addParameter(MQIACF_ACTION, it->first);
		}
	}
	if (action.compare("PubSub")) 
	{
		addParameter<std::string>(MQCA_CHILD, "ChildName");
		addParameter<std::string>(MQCA_PARENT, "ParentName");
	}
}

QueueManagerReset::~QueueManagerReset()
{
}

Poco::JSON::Array::Ptr QueueManagerReset::execute()
{
	PCFCommand::execute();

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

	for(PCF::Vector::const_iterator it = begin(); it != end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		Poco::JSON::Object::Ptr data = new Poco::JSON::Object();
		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
