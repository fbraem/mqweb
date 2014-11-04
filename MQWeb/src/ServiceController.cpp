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
#include "MQ/Web/ServiceController.h"
#include "MQ/Web/ServiceMapper.h"
#include "MQ/Web/JSONView.h"

namespace MQ
{
namespace Web
{

ServiceController::ServiceController() : MQController()
{
}


ServiceController::~ServiceController()
{

}


void ServiceController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("filter") && data().isObject("filter") )
	{
		pcfParameters = data().getObject("filter");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		set("filter", pcfParameters);

		std::vector<std::string> parameters = getParameters();
		// First parameter is queuemanager
		// Second parameter can be a servicename. If this is passed
		// the query parameter ServiceName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ServiceName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			std::string serviceNameField;
			if ( form().has("ServiceName") )
			{
				serviceNameField = form().get("ServiceName");
			}
			else if ( form().has("name") )
			{
				serviceNameField = form().get("name");
			}
			if ( serviceNameField.empty() )
			{
				serviceNameField = "*";
			}
			pcfParameters->set("ServiceName", serviceNameField);
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ServiceAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ServiceAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ServiceAttrs", attrs);
		}

		handleFilterForm(pcfParameters);
	}

	ServiceMapper mapper(*commandServer(), pcfParameters);
	set("services", mapper.inquire());
}


} } // Namespace MQ::Web
