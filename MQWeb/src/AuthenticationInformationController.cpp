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
#include "MQ/Web/AuthenticationInformationController.h"
#include "MQ/Web/AuthenticationInformationInquire.h"
#include "MQ/Web/AuthenticationInformationCreate.h"

namespace MQ
{
namespace Web
{


AuthenticationInformationController::AuthenticationInformationController() : MQController()
{
}


AuthenticationInformationController::~AuthenticationInformationController()
{
}


void AuthenticationInformationController::create()
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
		if (parameters.size() > 1)
		{
			pcfParameters->set("AuthInfoName", parameters[1]);
		}

		// Copy all query parameters to PCF, except AuthInfoName if it is already set on the URI
		for (Poco::Net::NameValueCollection::ConstIterator it = form().begin(); it != form().end(); ++it)
		{
			if (Poco::icompare(it->first, "Replace") == 0 ) continue;
			if (parameters.size() > 1 && Poco::icompare(it->first, "AuthInfoName") == 0) continue;

			pcfParameters->set(it->first, it->second);
		}
	}

	AuthenticationInformationCreate command(*commandServer(), pcfParameters, form().has("replace"));
	setData("data", command.execute());
}


void AuthenticationInformationController::inquire()
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
		// Second parameter can be a authentication information name (generic name is allowed)
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("AuthInfoName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			pcfParameters->set("AuthInfoName", form().get("AuthInfoName", "*"));
		}

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("AuthInfoAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for AuthInfoAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("AuthInfoAttrs", attrs);
		}

		if ( form().has("AuthInfoType") )
		{
			pcfParameters->set("AuthInfoType", form().get("AuthInfoType"));
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("QSGDisposition") )
		{
			pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

		handleFilterForm(pcfParameters);
	}

	AuthenticationInformationInquire command(*commandServer(), pcfParameters);
	setData("data", command.execute());
}


} } // Namespace MQ::Web
