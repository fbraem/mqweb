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
#include "MQ/Web/ProcessController.h"
#include "MQ/Web/ProcessMapper.h"

namespace MQ
{
namespace Web
{

ProcessController::ProcessController() : MQController()
{
}


ProcessController::~ProcessController()
{

}


void ProcessController::inquire()
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
		// Second parameter can be a processname. If this is passed
		// the query parameter ProcessName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ProcessName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			std::string processNameField;
			if ( form().has("ProcessName") )
			{
				processNameField = form().get("ProcessName");
			}
			else if ( form().has("name") )
			{
				processNameField = form().get("name");
			}
			if ( processNameField.empty() )
			{
				processNameField = "*";
			}
			pcfParameters->set("ProcessName", processNameField);
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ProcessAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ProcessAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ProcessAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("QSGDisposition") )
		{
			pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
		}

		handleFilterForm(pcfParameters);
	}

	ProcessMapper mapper(*commandServer(), pcfParameters);
	set("processes", mapper.inquire());
}


} } // Namespace MQ::Web
