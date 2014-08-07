/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they
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
#include "MQ/Web/JSONView.h"

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
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter can be a authentication information name and will result in inquiring 
	// only that queue and ignores all query parameters.
	if ( parameters.size() > 1 )
	{
		filter->set("profileName", parameters[1]);
	}
	else
	{
		// Handle query parameters
		if ( form().has("profileName") ) filter->set("profileName", form().get("profileName"));
	}

	Poco::JSON::Array::Ptr jsonOptions = new Poco::JSON::Array();
	filter->set("options", jsonOptions);
	for(Poco::Net::NameValueCollection::ConstIterator itOptions = form().find("options"); 
		itOptions != form().end() && itOptions->first == "options"; 
		++itOptions)
	{
		jsonOptions->add(itOptions->second);
	}

	if ( form().has("objectType") ) filter->set("objectType", form().get("objectType"));
	if ( form().has("entityName") ) filter->set("entityName", form().get("entityName"));
	if ( form().has("entityType") ) filter->set("entityType", form().get("entityType"));
	if ( form().has("serviceComponent") ) filter->set("serviceComponent", form().get("serviceComponent"));

	AuthorityRecordMapper mapper(*commandServer());
	Poco::JSON::Array::Ptr json = mapper.inquire(filter);
	set("authrecs", json);
	setView(new JSONView());
}


} } // Namespace MQ::Web
