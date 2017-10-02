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
#include "MQ/Web/SubscriptionStatusController.h"
#include "MQ/Web/SubscriptionStatusInquire.h"

namespace MQ
{
namespace Web
{

SubscriptionStatusController::SubscriptionStatusController() : MQController()
{
}


SubscriptionStatusController::~SubscriptionStatusController()
{

}


void SubscriptionStatusController::inquire()
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
		// Second parameter can be a subname. If this is passed
		// the query parameter subName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("SubName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			pcfParameters->set("SubName", form().get("SubName", "*"));
		}

		if ( form().has("SubId") )
		{
			pcfParameters->set("SubId", form().get("SubId"));
		}

		if ( form().has("SubscriptionType") )
		{
			pcfParameters->set("SubscriptionType", form().get("SubscriptionType"));
		}

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("StatusAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for StatusAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("StatusAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("Durable") )
		{
			pcfParameters->set("Durable", form().get("Durable"));
		}

		handleFilterForm(pcfParameters);
	}

	SubscriptionStatusInquire command(*commandServer(), pcfParameters);
	set("data", command.execute());
}


} } // Namespace MQ::Web
