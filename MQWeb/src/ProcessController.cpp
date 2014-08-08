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
#include "MQ/Web/JSONView.h"

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
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter can be a processname and will result in inquiring
	// only that process.
	if ( parameters.size() > 1 )
	{
		filter->set("name", parameters[1]);
	}
	else
	{
		std::string processNameField = form().get("processName", "*");
		filter->set("name", processNameField.empty() ? "*" : processNameField);
		filter->set("excludeSystem", form().get("excludeSystem", "false").compare("true") == 0);
	}

	ProcessMapper mapper(*commandServer());
	Poco::JSON::Array::Ptr json = mapper.inquire(filter);
	set("processes", json);
}


} } // Namespace MQ::Web
