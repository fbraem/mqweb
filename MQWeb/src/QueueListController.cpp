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
#include <Poco/Net/HTMLForm.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/JSON/TemplateCache.h>

#include <MQ/MQException.h>
#include <MQ/CommandServer.h>
#include <MQ/Web/QueueListController.h>
#include <MQ/Web/QueueMQMapper.h>

namespace MQ
{
namespace Web
{

QueueListController::QueueListController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : Controller(request, response)
{
}


QueueListController::~QueueListController()
{

}


void QueueListController::handle()
{
	if ( _request.getMethod().compare("POST") != 0 )
	{
		_response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	Poco::Net::HTMLForm form(_request, _request.stream());

	std::string queueNameField = form.get("queueName", "*");
	if ( queueNameField.empty() )
	{
		queueNameField = "*";
	}
	filter->set("name", queueNameField);

	std::string queueDepthField = form.get("queueDepth", "");
	int queueDepth;
	if ( Poco::NumberParser::tryParse(queueDepthField, queueDepth) )
	{
		filter->set("qdepth", queueDepth);
	}

	filter->set("type", form.get("queueType", "All"));
	filter->set("excludeSystem", form.get("queueExcludeSystem", "0").compare("1") == 0);

	QueueMQMapper queueMapper(*_commandServer.get());
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

	_data->set("queues", jsonQueues);

	Poco::JSON::Template::Ptr tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path("queueList.tpl"));
	poco_assert_dbg(!tpl.isNull());

	tpl->render(_data, _response.send());
	_response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
}

} } // Namespace MQ::Web
