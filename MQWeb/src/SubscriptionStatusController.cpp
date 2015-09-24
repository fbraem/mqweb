/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
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
#include "MQ/Web/SubscriptionStatusController.h"
#include "MQ/Web/SubscriptionStatusMapper.h"

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
		// Second parameter can be a subname. If this is passed
		// the query parameter subName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("SubName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			std::string subNameField;
			if ( form().has("SubName") )
			{
				subNameField = form().get("SubName");
			}
			else if ( form().has("name") )
			{
				subNameField = form().get("name");
			}
			pcfParameters->set("SubName", subNameField);
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

	SubscriptionStatusMapper mapper(*commandServer(), pcfParameters);
	set("statuses", mapper.inquire());
}


} } // Namespace MQ::Web
