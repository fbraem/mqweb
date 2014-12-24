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

	if ( data().has("filter") && data().isObject("filter") )
	{
		pcfParameters = data().getObject("filter");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		set("filter", pcfParameters);

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
	set("authservices", mapper.inquire());
}


} } // Namespace MQ::Web
