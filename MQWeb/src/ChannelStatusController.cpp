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
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>

#include <MQ/Web/MQController.h>
#include <MQ/Web/ChannelStatusController.h>
#include <MQ/Web/ChannelMapper.h>
#include <MQ/Web/ChannelStatusMapper.h>
#include <MQ/Web/TemplateView.h>
#include <MQ/Web/JSONView.h>

namespace MQ
{
namespace Web
{

ChannelStatusController::ChannelStatusController() : MQController()
{
}


ChannelStatusController::~ChannelStatusController()
{

}


void ChannelStatusController::list()
{
	Poco::Net::HTMLForm form(request(), request().stream());

	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();
	std::string channelNameField = form.get("channelName", "*");
	filter->set("name", channelNameField.empty() ? "*" : channelNameField);
	filter->set("type", form.get("channelType", "All"));
	ChannelStatusMapper channelStatusMapper(*commandServer());
	Poco::JSON::Array::Ptr statuses = channelStatusMapper.inquire(filter);

	set("statuses", statuses);

	if ( format().compare("html") == 0 )
	{
		setView(new TemplateView("channelStatus.tpl"));
	}
	else if ( format().compare("json") == 0 )
	{
		setView(new JSONView());
	}

}


void ChannelStatusController::view()
{
/*
	std::vector<std::string> parameters = getParameters();

	// First parameter is the queuemanager name
	// Second parameter is the channelname
	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return;
	}

	std::string channelName = parameters[1];

	std::string type = form().get("type", "All");

	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();
	filter->set("name", channelName);
	filter->set("type", type);

	ChannelMapper channelMapper(*commandServer());
	Poco::JSON::Array::Ptr jsonChannels = channelMapper.inquire(filter);
	Poco::JSON::Object::Ptr jsonChannel = jsonChannels->getObject(0);

	if ( !jsonChannel.isNull() )
	{
		set("channel", jsonChannel);
	}
	render("channel.tpl");
*/
}


} } // Namespace MQ::Web
