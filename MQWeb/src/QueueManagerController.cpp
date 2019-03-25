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
#include "MQ/Web/QueueManagerController.h"
#include "MQ/Web/QueueManagerInquire.h"
#include "MQ/Web/QueueManagerPing.h"
#include "MQ/Web/QueueManagerReset.h"

namespace MQ
{
namespace Web
{


QueueManagerController::QueueManagerController() : MQController()
{
}


QueueManagerController::~QueueManagerController()
{
}


void QueueManagerController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("input") && data().isObject("input") )
	{
		// Posted JSON
		pcfParameters = data().getObject("input");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		meta().set("input", pcfParameters);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("QMgrAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for QMgrAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("QMgrAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}
	}

	QueueManagerInquire command(*commandServer(), pcfParameters);
	setData("data", command.execute());
}

void QueueManagerController::ping()
{
	Poco::JSON::Object::Ptr pcfParameters = new Poco::JSON::Object();
	meta().set("input", pcfParameters);

	QueueManagerPing command(*commandServer(), pcfParameters);
	setData("data", command.execute());
}

void QueueManagerController::reset()
{
	Poco::JSON::Object::Ptr pcfParameters = new Poco::JSON::Object();
	meta().set("input", pcfParameters);

	std::vector<std::string> parameters = getParameters();
	if (parameters.size() > 1)
	{
		pcfParameters->set("Action", parameters[1]);
	}
	else
	{
		// Handle query parameters
		if (form().has("Action"))
		{
			pcfParameters->set("Action", form().get("Action"));
		}
	}

	if (form().has("ChildName"))
	{
		pcfParameters->set("ChildName", form().get("ChildName"));
	}
	if (form().has("Parent"))
	{
		pcfParameters->set("Parent", form().get("Parent"));
	}

	QueueManagerReset command(*commandServer(), pcfParameters);
	setData("data", command.execute());
}

} } // Namespace MQ::Web
