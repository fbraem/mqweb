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
#include "MQ/Web/ConnectionController.h"
#include "MQ/Web/ConnectionMapper.h"

namespace MQ
{
namespace Web
{

ConnectionController::ConnectionController() : MQController()
{
}


ConnectionController::~ConnectionController()
{

}


void ConnectionController::inquire()
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
		// Second parameter can be a connection id and will result in inquiring
		// only that connection.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ConnectionId", parameters[1]);
		}

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ConnectionAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ConnectionAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ConnectionAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("ConnInfoType") )
		{
			pcfParameters->set("ConnInfoType", form().get("ConnInfoType"));
		}

		if ( form().has("URDisposition") )
		{
			pcfParameters->set("URDisposition", form().get("URDisposition"));
		}

		handleFilterForm(pcfParameters);
	}

	ConnectionMapper mapper(*commandServer(), pcfParameters);
	set("connections", mapper.inquire());
}


} } // Namespace MQ::Web
