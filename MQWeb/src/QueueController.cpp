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
#include "Poco/DateTimeFormatter.h"
#include "Poco/URI.h"
#include "Poco/Net/HTMLForm.h"

#include "MQ/Web/QueueController.h"
#include "MQ/Web/QueueMapper.h"
#include "MQ/Web/MultiView.h"
#include "MQ/Web/JSONView.h"

namespace MQ
{
namespace Web
{


QueueController::QueueController() : MQController()
{
}


QueueController::~QueueController()
{
}


void QueueController::index()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("queue/head.tpl"));
	multiView->add("main", new TemplateView("queue/index.tpl"));
	setView(multiView);
}


void QueueController::inquire()
{
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter can be a queuename and will result in inquiring 
	// only that queue and ignores all query parameters.
	if ( parameters.size() > 1 )
	{
		filter->set("name", parameters[1]);
	}
	else
	{
		// Handle query parameters
		std::string queueNameField = form().get("queueName", "*");
		if ( queueNameField.empty() )
		{
			queueNameField = "*";
		}
		filter->set("name", queueNameField);

		std::string queueDepthField = form().get("queueDepth", "");
		int queueDepth = 0;
		if ( Poco::NumberParser::tryParse(queueDepthField, queueDepth) )
		{
			filter->set("qdepth", queueDepth);
		}

		if ( form().has("queueUsage") )
		{
			filter->set("usage", form().get("queueUsage"));
		}

		filter->set("type", form().get("queueType", "All"));
		filter->set("excludeSystem", form().get("queueExcludeSystem", "false").compare("true") == 0);
		filter->set("excludeTemp", form().get("queueExcludeTemp", "false").compare("true") == 0);
	}

	QueueMapper queueMapper(*commandServer());
	Poco::JSON::Array::Ptr jsonQueues = queueMapper.inquire(filter);
	set("queues", jsonQueues);
	setView(new JSONView());
}


} } // Namespace MQ::Web
