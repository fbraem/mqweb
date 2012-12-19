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
#include <Poco/JSON/TemplateCache.h>

#include <MQ/MQException.h>
#include <MQ/CommandServer.h>

#include <MQ/Web/QueueController.h>
#include <MQ/Web/QueueMQMapper.h>

namespace MQ
{
namespace Web
{

QueueController::QueueController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : Controller(request, response)
{
}


QueueController::~QueueController()
{

}


void QueueController::handle()
{
	Poco::URI uri(_request.getURI());
	std::vector<std::string> paths;
	uri.getPathSegments(paths);

	if ( paths.size() < 3 )
	{
		_response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string queueName = paths[2];

	Poco::JSON::Object::Ptr jsonQmgr = _data->getObject("qmgr");
	poco_assert_dbg(!jsonQmgr.isNull());

	jsonQmgr->set("createDate", Poco::DateTimeFormatter::format(_qmgr->creationDate(), "%d-%m-%Y %H:%M:%S"));
	jsonQmgr->set("alterDate", Poco::DateTimeFormatter::format(_qmgr->alterationDate(), "%d-%m-%Y %H:%M:%S"));
	jsonQmgr->set("id", _qmgr->id());
	jsonQmgr->set("description", _qmgr->description());

	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();
	filter->set("name", queueName);

	QueueMQMapper queueMapper(*_commandServer.get());
	Poco::JSON::Array::Ptr jsonQueues = queueMapper.inquire(filter);

	if ( jsonQueues->size() > 0 )
	{
		Poco::JSON::Object::Ptr jsonQueue = jsonQueues->getObject(0);
		if ( ! jsonQueue.isNull() )
		{
			_data->set("queue", jsonQueues->getObject(0));

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
			Poco::JSON::Template::Ptr tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path("queue.tpl"));
			poco_assert_dbg(!tpl.isNull());

			tpl->render(_data, _response.send());
			_response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);

			return;
		}
	}

	_response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
}


} } // Namespace MQ::Web
