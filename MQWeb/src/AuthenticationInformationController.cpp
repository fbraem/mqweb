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
#include "MQ/Web/JSONView.h"

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
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter can be a authentication information name and will result in inquiring 
	// only that queue and ignores all query parameters.
	if ( parameters.size() > 1 )
	{
		filter->set("authInfoName", parameters[1]);
	}
	else
	{
		// Handle query parameters
		std::string authInfoNameField = form().get("authInfoName", "*");
		if ( authInfoNameField.empty() )
		{
			authInfoNameField = "*";
		}
		filter->set("authInfoName", authInfoNameField);

		filter->set("excludeSystem", form().get("excludeSystem", "false").compare("true") == 0);
	}

	AuthenticationInformationMapper mapper(*commandServer());
	Poco::JSON::Array::Ptr json = mapper.inquire(filter);
	set("authinfos", json);
}


} } // Namespace MQ::Web
