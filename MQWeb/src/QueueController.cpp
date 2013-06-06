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
#include <Poco/DateTimeFormatter.h>
#include <Poco/URI.h>
#include <Poco/Net/HTMLForm.h>

#include <MQ/Web/QueueController.h>
#include <MQ/Web/QueueMapper.h>
#include <MQ/Web/MultiView.h>
#include <MQ/Web/JSONView.h>

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


void QueueController::list()
{
	if ( format().compare("html") == 0 )
	{
		response().redirect("/queue/index/" + qmgr()->name());
		return;
	}

	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::string queueNameField = form().get("queueName", "*");
	if ( queueNameField.empty() )
	{
		queueNameField = "*";
	}
	filter->set("name", queueNameField);

	std::string queueDepthField = form().get("queueDepth", "");
	int queueDepth;
	if ( Poco::NumberParser::tryParse(queueDepthField, queueDepth) )
	{
		filter->set("qdepth", queueDepth);
	}

	if ( form().has("queueUsage") )
	{
		filter->set("usage", form().get("queueUsage"));
	}

	filter->set("type", form().get("queueType", "All"));
	filter->set("excludeSystem", form().get("queueExcludeSystem", "0").compare("1") == 0);
	filter->set("excludeTemp", form().get("queueExcludeTemp", "0").compare("1") == 0);

	QueueMapper queueMapper(*commandServer());
	Poco::JSON::Array::Ptr jsonQueues = queueMapper.inquire(filter);

	for(int i = 0; i < jsonQueues->size(); ++i)
	{
		Poco::JSON::Object::Ptr jsonQueue = jsonQueues->getObject(i);
		if ( ! jsonQueue.isNull() )
		{
			Poco::JSON::Object::Ptr jsonType = jsonQueue->getObject("QType");
			if ( !jsonType.isNull() )
			{
				// Add a property with the type as propertyname and true as value
				// to help a view to check which type of queue we have. For example:
				// {
				//   "QType" : { "value" : 1, "display" : "Local", "Local" : true }
				// }
				std::string display = jsonType->optValue<std::string>("display", "");
				if ( ! display.empty() )
				{
					jsonType->set(display, true);
				}
			}
		}
	}

	set("queues", jsonQueues);

	if ( format().compare("html") == 0 )
	{
		//setView(new TemplateView("queue/queues.tpl"));
		//TODO: redirect to index action
	}
	else if ( format().compare("json") == 0 )
	{
		setView(new JSONView());
	}
}


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

	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();
	filter->set("name", queueName);

	QueueMapper queueMapper(*commandServer());
	Poco::JSON::Array::Ptr jsonQueues = queueMapper.inquire(filter);

	if ( jsonQueues->size() > 0 )
	{
		Poco::JSON::Object::Ptr jsonQueue = jsonQueues->getObject(0);
		if ( ! jsonQueue.isNull() )
		{
			set("queue", jsonQueues->getObject(0));

			Poco::JSON::Object::Ptr jsonType = jsonQueue->getObject("QType");
			if ( !jsonType.isNull() )
			{
				// Add a property with the type as propertyname and true as value
				// to help a view to check which type of queue we have. For example:
				// {
				//   "QType" : { "value" : 1, "display" : "Local", "Local" : true }
				// }
				std::string display = jsonType->optValue<std::string>("display", "");
				if ( ! display.empty() )
				{
					jsonType->set(display, true);
				}
			}

			setView(new TemplateView("queue/view.tpl"));

			return;
		}
	}

	setResponseStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
}


} } // Namespace MQ::Web
