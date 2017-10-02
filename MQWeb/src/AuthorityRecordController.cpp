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
#include "MQ/Web/AuthorityRecordController.h"
#include "MQ/Web/AuthorityRecordInquire.h"

namespace MQ
{
namespace Web
{


AuthorityRecordController::AuthorityRecordController() : MQController()
{
}


AuthorityRecordController::~AuthorityRecordController()
{
}


void AuthorityRecordController::inquire()
{
	if ( qmgr()->zos() )
	{
		// Authority records are not supported on z/OS
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_NOT_IMPLEMENTED, "/authrec/inquire not implemented for z/OS");
		return;
	}

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
		// Second parameter can be a authentication information name and will result in inquiring
		// only that queue and ignores all query parameters.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ProfileName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			if ( form().has("ProfileName") )
			{
				pcfParameters->set("ProfileName", form().get("ProfileName"));
			}
		}

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ProfileAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ProfileAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ProfileAttrs", attrs);
		}

		Poco::JSON::Array::Ptr jsonOptions = new Poco::JSON::Array();
		pcfParameters->set("Options", jsonOptions);
		for(Poco::Net::NameValueCollection::ConstIterator itOptions = form().find("Options");
			itOptions != form().end() && Poco::icompare(itOptions->first, "Options") == 0;
			++itOptions)
		{
			jsonOptions->add(itOptions->second);
		}

		if ( form().has("ObjectType") ) pcfParameters->set("ObjectType", form().get("ObjectType"));
		if ( form().has("EntityName") ) pcfParameters->set("EntityName", form().get("EntityName"));
		if ( form().has("EntityType") ) pcfParameters->set("EntityType", form().get("EntityType"));
		if ( form().has("ServiceComponent") ) pcfParameters->set("ServiceComponent", form().get("ServiceComponent"));
	}

	AuthorityRecordInquire command(*commandServer(), pcfParameters);
	set("data", command.execute());
}


} } // Namespace MQ::Web
