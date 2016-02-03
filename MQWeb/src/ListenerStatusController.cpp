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
#include "MQ/Web/ListenerStatusController.h"
#include "MQ/Web/ListenerStatusMapper.h"

namespace MQ
{
namespace Web
{


ListenerStatusController::ListenerStatusController() : MQController()
{
}


ListenerStatusController::~ListenerStatusController()
{
}


void ListenerStatusController::inquire()
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
		// Second parameter can be a listenername and will result in inquiring 
		// only that listener.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ListenerName", parameters[1]);
		}
		else
		{
			std::string listenerNameField;
			if ( form().has("ListenerName") )
			{
				listenerNameField = form().get("ListenerName");
			}
			else if ( form().has("name") )
			{
				listenerNameField = form().get("name");
			}
			if ( listenerNameField.empty() )
			{
				listenerNameField = "*";
			}
			pcfParameters->set("ListenerName", listenerNameField);
		}
	}

	Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
	formElementToJSONArray("ListenerStatusAttrs", attrs);
	if ( attrs->size() == 0 ) // Nothing found for ListenerStatusAttrs, try Attrs
	{
		formElementToJSONArray("Attrs", attrs);
	}
	if ( attrs->size() > 0 )
	{
		pcfParameters->set("ListenerStatusAttrs", attrs);
	}

	pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

	handleFilterForm(pcfParameters);

	ListenerStatusMapper mapper(*commandServer(), pcfParameters);
	set("statuses", mapper.inquire());
}


} } // Namespace MQ::Web
