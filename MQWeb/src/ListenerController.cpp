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
#include "MQ/Web/ListenerController.h"
#include "MQ/Web/ListenerMapper.h"

namespace MQ
{
namespace Web
{


ListenerController::ListenerController() : MQController()
{
}


ListenerController::~ListenerController()
{
}


void ListenerController::inquire()
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

		if ( parameters.size() > 2 )
		{
			pcfParameters->set("TransportType", parameters[2]);
		}
		else if ( form().has("TransportType") )
		{
			pcfParameters->set("TransportType", form().get("TransportType"));
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ListenerAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ListenerAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ListenerAttrs", attrs);
		}

		handleFilterForm(pcfParameters);
	}

	ListenerMapper mapper(*commandServer(), pcfParameters);
	set("listeners", mapper.inquire());
}


} } // Namespace MQ::Web
