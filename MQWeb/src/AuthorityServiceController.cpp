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
#include "MQ/Web/AuthorityServiceController.h"
#include "MQ/Web/AuthorityServiceMapper.h"

namespace MQ
{
namespace Web
{


AuthorityServiceController::AuthorityServiceController() : MQController()
{
}


AuthorityServiceController::~AuthorityServiceController()
{
}


void AuthorityServiceController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("input") && data().isObject("input") )
	{
		pcfParameters = data().getObject("input");
		// There is a bug in MQCMD_INQUIRE_AUTH_SERVICE, AuthServiceAttrs is required!
		if ( !pcfParameters->has("AuthServiceAttrs") )
		{
			Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
			attrs->add("All");
			pcfParameters->set("AuthServiceAttrs", attrs);
		}
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		set("input", pcfParameters);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("AuthServiceAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for AuthServiceAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() == 0 ) // Default must be specified for this command!
		{
			attrs->add("All");
		}
		pcfParameters->set("AuthServiceAttrs", attrs);

		if ( form().has("ServiceComponent") )
		{
			pcfParameters->set("ServiceComponent", form().get("ServiceComponent"));
		}

		handleFilterForm(pcfParameters);
	}

	AuthorityServiceMapper mapper(*commandServer(), pcfParameters);
	set("data", mapper.inquire());
}


} } // Namespace MQ::Web
