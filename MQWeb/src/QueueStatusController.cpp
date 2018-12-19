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
#include "MQ/Web/QueueStatusController.h"
#include "MQ/Web/QueueStatusInquire.h"

namespace MQ
{
namespace Web
{


QueueStatusController::QueueStatusController() : MQController()
{
}


QueueStatusController::~QueueStatusController()
{
}


void QueueStatusController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("input") && data().isObject("input") )
	{
		pcfParameters = data().getObject("input");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		setData("input", pcfParameters);

		std::vector<std::string> parameters = getParameters();
		// First parameter is queuemanager
		// Second parameter can be a queuename. If this is passed, the
		// query parameter QName or queueName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("QName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			pcfParameters->set("QName", form().get("QName", "*"));
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);
		pcfParameters->set("ExcludeTemp", form().get("ExcludeTemp", "false").compare("true") == 0);

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}
		if ( form().has("QSGDisposition") )
		{
			pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
		}

		if ( form().has("StatusType") ) pcfParameters->set("StatusType", form().get("StatusType"));
		if ( form().has("OpenType") ) pcfParameters->set("OpenType", form().get("OpenType"));

		handleFilterForm(pcfParameters);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("QStatusAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for QStatusAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("QStatusAttrs", attrs);
		}

	}

	QueueStatusInquire command(*commandServer(), pcfParameters);
	setData("data", command.execute());
}


} } // Namespace MQ::Web
