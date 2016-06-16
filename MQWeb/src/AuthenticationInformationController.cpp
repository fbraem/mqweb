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
#include "MQ/Web/AuthenticationInformationController.h"
#include "MQ/Web/AuthenticationInformationMapper.h"

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
		set("input", pcfParameters);

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
			std::string authInfoNameField;
			if ( form().has("AuthInfoName") )
			{
				authInfoNameField = form().get("AuthInfoName");
			}
			else if ( form().has("name") )
			{
				authInfoNameField = form().get("name");
			}
			if ( authInfoNameField.empty() )
			{
				authInfoNameField = "*";
			}
			pcfParameters->set("AuthInfoName", authInfoNameField);
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

	AuthenticationInformationMapper mapper(*commandServer(), pcfParameters);
	set("data", mapper.inquire());
}


} } // Namespace MQ::Web
