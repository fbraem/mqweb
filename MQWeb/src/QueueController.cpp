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

/**
 * URL: queue/index/<qmgrName>
 *
 * Shows a form for displaying queues.
 * Only HTML format is supported.
 */
void QueueController::index()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("queue/head.tpl"));
	multiView->add("main", new TemplateView("queue/index.tpl"));
	setView(multiView);
}

/**
 * URL: queue/inquire/<qmgrName>
 *
 * Query Parameters:
 *   + queueName: Name of the queue (* is default).
 *   + queueDepth: Only select queues which has at least queueDepth messages.
 *   + queueUsage: xmitq or normal (default is normal)
 *   + type: queue type. Possible values: All, Local, Alias, Cluster, Model or Remote (default is All)
 *
 * Inquire queues. Only JSON format is available.
 */
void QueueController::inquire()
{
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

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

	QueueMapper queueMapper(*commandServer());
	Poco::JSON::Array::Ptr jsonQueues = queueMapper.inquire(filter);
	set("queues", jsonQueues);
	setView(new JSONView());
}


/**
 * URL: queue/view/<qmgrName>/<queueName>
 *
 * Returns HTML to view details of the given queue
 */
void QueueController::view()
{
	std::vector<std::string> parameters = getParameters();

	// First parameter is the queuemanager name
	// Second parameter is the queue name
	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string queueName = parameters[1];

	set("queueName", queueName);
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("queue/view_head.tpl"));
	multiView->add("main", new TemplateView("queue/view.tpl"));
	setView(multiView);
	return;
}


} } // Namespace MQ::Web
