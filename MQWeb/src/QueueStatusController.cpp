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
#include "MQ/Web/QueueStatusController.h"
#include "MQ/Web/QueueStatusMapper.h"
#include "MQ/Web/JSONView.h"

namespace MQ
{
namespace Web
{


QueueStatusController::QueueStatusController() : MQController()
{
}


QueueStatusController::~QueueStatusController()
{
}


void QueueStatusController::inquire()
{
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter can be a queuename and will result in inquiring 
	// only that queue
	if ( parameters.size() > 1 )
	{
		filter->set("queueName", parameters[1]);
	}
	else
	{
		if ( form().has("queueName") ) 
		{
			std::string nameField = form().get("queueName");
			if ( nameField.empty() ) nameField = "*";
			filter->set("queueName", nameField);
		}
		filter->set("excludeSystem", form().get("excludeSystem", "false").compare("true") == 0);
		filter->set("excludeTemp", form().get("excludeTemp", "false").compare("true") == 0);
	}

	if ( form().has("statusType") ) filter->set("statusType", form().get("statusType"));
	if ( form().has("openType") ) filter->set("openType", form().get("openType"));

	QueueStatusMapper mapper(*commandServer());
	Poco::JSON::Array::Ptr json = mapper.inquire(filter);
	set("statuses", json);
	setView(new JSONView());
}


} } // Namespace MQ::Web
