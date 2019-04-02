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
#include "MQ/Web/AuthorityRecordSet.h"
#include "MQ/Web/AuthorityRecordRemove.h"

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
		setData("input", pcfParameters);

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
	setData("data", command.execute());
}


void AuthorityRecordController::set()
{
	if ( qmgr()->zos() )
	{
		// Authority records are not supported on z/OS
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_NOT_IMPLEMENTED, "/authrec/set not implemented for z/OS");
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
		setData("input", pcfParameters);

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

		if ( form().has("ObjectType") ) pcfParameters->set("ObjectType", form().get("ObjectType"));

		Poco::JSON::Array::Ptr jsonAdd = new Poco::JSON::Array();
		pcfParameters->set("AuthorityAdd", jsonAdd);
		if ( form().has("AuthorityAdd") )
		{
			for(Poco::Net::NameValueCollection::ConstIterator itAuthority = form().find("AuthorityAdd");
				itAuthority != form().end() && Poco::icompare(itAuthority->first, "AuthorityAdd") == 0;
				++itAuthority)
			{
				jsonAdd->add(itAuthority->second);
			}
		}

		Poco::JSON::Array::Ptr jsonRemove = new Poco::JSON::Array();
		pcfParameters->set("AuthorityRemove", jsonRemove);
		if ( form().has("AuthorityRemove") )
		{
			for(Poco::Net::NameValueCollection::ConstIterator itAuthority = form().find("AuthorityRemove");
				itAuthority != form().end() && Poco::icompare(itAuthority->first, "AuthorityRemove") == 0;
				++itAuthority)
			{
				jsonRemove->add(itAuthority->second);
			}
		}

		if (form().has("GroupNames"))
		{
			Poco::JSON::Array::Ptr jsonGroupNames = new Poco::JSON::Array();
			pcfParameters->set("GroupNames", jsonGroupNames);
			for(Poco::Net::NameValueCollection::ConstIterator itGroupNames = form().find("GroupNames");
				itGroupNames != form().end() && Poco::icompare(itGroupNames->first, "GroupNames") == 0;
				++itGroupNames)
			{
				jsonGroupNames->add(itGroupNames->second);
			}
		}

		if (form().has("PrincipalNames"))
		{
			Poco::JSON::Array::Ptr jsonPrincipalNames = new Poco::JSON::Array();
			pcfParameters->set("PrincipalNames", jsonPrincipalNames);
			for(Poco::Net::NameValueCollection::ConstIterator itPrincipalNames = form().find("PrincipalNames");
				itPrincipalNames != form().end() && Poco::icompare(itPrincipalNames->first, "PrincipalNames") == 0;
				++itPrincipalNames)
			{
				jsonPrincipalNames->add(itPrincipalNames->second);
			}
		}

		if ( form().has("ServiceComponent") ) pcfParameters->set("ServiceComponent", form().get("ServiceComponent"));
	}

	AuthorityRecordSet command(*commandServer(), pcfParameters);
	setData("data", command.execute());
}


void AuthorityRecordController::remove()
{
	if ( qmgr()->zos() )
	{
		// Authority records are not supported on z/OS
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_NOT_IMPLEMENTED, "/authrec/remove not implemented for z/OS");
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
		setData("input", pcfParameters);

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

		if ( form().has("ObjectType") ) pcfParameters->set("ObjectType", form().get("ObjectType"));

		if (form().has("GroupNames"))
		{
			Poco::JSON::Array::Ptr jsonGroupNames = new Poco::JSON::Array();
			pcfParameters->set("GroupNames", jsonGroupNames);
			for(Poco::Net::NameValueCollection::ConstIterator itGroupNames = form().find("GroupNames");
				itGroupNames != form().end() && Poco::icompare(itGroupNames->first, "GroupNames") == 0;
				++itGroupNames)
			{
				jsonGroupNames->add(itGroupNames->second);
			}
		}

		if (form().has("PrincipalNames"))
		{
			Poco::JSON::Array::Ptr jsonPrincipalNames = new Poco::JSON::Array();
			pcfParameters->set("PrincipalNames", jsonPrincipalNames);
			for(Poco::Net::NameValueCollection::ConstIterator itPrincipalNames = form().find("PrincipalNames");
				itPrincipalNames != form().end() && Poco::icompare(itPrincipalNames->first, "PrincipalNames") == 0;
				++itPrincipalNames)
			{
				jsonPrincipalNames->add(itPrincipalNames->second);
			}
		}
	}

	AuthorityRecordRemove command(*commandServer(), pcfParameters);
	setData("data", command.execute());
}

} } // Namespace MQ::Web
