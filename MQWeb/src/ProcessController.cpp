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
#include "MQ/Web/ProcessController.h"
#include "MQ/Web/ProcessInquire.h"

namespace MQ
{
namespace Web
{

ProcessController::ProcessController() : MQController()
{
}


ProcessController::~ProcessController()
{

}


void ProcessController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("input") && data().isObject("input") )
	{
		pcfParameters = data().getObject("input");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		set("input", pcfParameters);

		std::vector<std::string> parameters = getParameters();
		// First parameter is queuemanager
		// Second parameter can be a processname. If this is passed
		// the query parameter ProcessName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ProcessName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			pcfParameters->set("ProcessName", form().get("ProcessName", "*"));
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ProcessAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ProcessAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ProcessAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("QSGDisposition") )
		{
			pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
		}

		handleFilterForm(pcfParameters);
	}

	ProcessInquire command(*commandServer(), pcfParameters);
	set("data", command.execute());
}


} } // Namespace MQ::Web
