/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
 * will be approved by the European Commission - subsequent
 * versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 * Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://joinup.ec.europa.eu/software/page/eupl
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the Licence is
 * distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the Licence for the specific language governing
 * permissions and limitations under the Licence.
 */
#include "MQ/Web/AuthorityRecordController.h"
#include "MQ/Web/AuthorityRecordMapper.h"

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
			else if ( form().has("Name") )
			{
				pcfParameters->set("ProfileName", form().get("Name"));
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

	AuthorityRecordMapper mapper(*commandServer(), pcfParameters);
	set("authrecs", mapper.inquire());
}


} } // Namespace MQ::Web
