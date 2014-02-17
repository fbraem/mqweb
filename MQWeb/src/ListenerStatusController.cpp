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
#include "MQ/Web/MultiView.h"
#include "MQ/Web/JSONView.h"

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


void ListenerStatusController::index()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("listener/head.tpl"));
	multiView->add("main", new TemplateView("listener/index.tpl"));
	setView(multiView);
}


void ListenerStatusController::inquire()
{
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter can be a listenername and will result in inquiring 
	// only that listener and ignores all query parameters.
	if ( parameters.size() > 1 )
	{
		filter->set("name", parameters[1]);
	}
	else
	{
		// Handle query parameters
		std::string listenerNameField = form().get("listenerName", "*");
		if ( listenerNameField.empty() )
		{
			listenerNameField = "*";
		}
		filter->set("name", listenerNameField);
	}

	ListenerStatusMapper mapper(*commandServer());
	Poco::JSON::Array::Ptr statuses = mapper.inquire(filter);
	set("statuses", statuses);
	setView(new JSONView());
}


} } // Namespace MQ::Web
