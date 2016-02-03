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
#include "MQ/Web/NamelistController.h"
#include "MQ/Web/NamelistMapper.h"

namespace MQ
{
namespace Web
{

NamelistController::NamelistController() : MQController()
{
}


NamelistController::~NamelistController()
{

}


void NamelistController::inquire()
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
		// Second parameter can be a namelistname and will result in inquiring 
		// only that namelist.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("NamelistName", parameters[1]);
		}
		else
		{
			std::string namelistNameField;
			if ( form().has("NamelistName") )
			{
				namelistNameField = form().get("NamelistName");
			}
			else if ( form().has("name") )
			{
				namelistNameField = form().get("name");
			}
			if ( namelistNameField.empty() )
			{
				namelistNameField = "*";
			}
			pcfParameters->set("NamelistName", namelistNameField);
		}

		if ( parameters.size() > 2 )
		{
			pcfParameters->set("NamelistType", parameters[2]);
		}
		else if ( form().has("NamelistType") )
		{
			pcfParameters->set("NamelistType", form().get("NamelistType"));
		}
	}

	if ( form().has("CommandScope") )
	{
		pcfParameters->set("CommandScope", form().get("CommandScope"));
	}

	if ( form().has("QSGDisposition") )
	{
		pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
	}

	Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
	formElementToJSONArray("NamelistAttrs", attrs);
	if ( attrs->size() == 0 ) // Nothing found for ListenerStatusAttrs, try Attrs
	{
		formElementToJSONArray("Attrs", attrs);
	}
	if ( attrs->size() > 0 )
	{
		pcfParameters->set("NamelistAttrs", attrs);
	}

	handleFilterForm(pcfParameters);

	pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

	NamelistMapper mapper(*commandServer(), pcfParameters);
	set("namelists", mapper.inquire());
}


} } // Namespace MQ::Web
