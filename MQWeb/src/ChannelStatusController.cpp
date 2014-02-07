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
#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URI.h"

#include "MQ/Web/MQController.h"
#include "MQ/Web/ChannelStatusController.h"
#include "MQ/Web/ChannelMapper.h"
#include "MQ/Web/ChannelStatusMapper.h"
#include "MQ/Web/MultiView.h"
#include "MQ/Web/JSONView.h"

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


void ChannelStatusController::index()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("chstatus/head.tpl"));
	multiView->add("main", new TemplateView("chstatus/index.tpl"));
	setView(multiView);
}


void ChannelStatusController::inquire()
{
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter can be a channelname and will result in inquiring
	// only that channel. A third parameter is required because we need
	// also the type of the channel for inquiring a specific channel.
	if ( parameters.size() > 1 )
	{
		filter->set("name", parameters[1]);
	}
	else
	{
		Poco::Net::HTMLForm form(request(), request().stream());
		std::string channelNameField = form.get("channelName", "*");
		filter->set("name", channelNameField.empty() ? "*" : channelNameField);
	}

	ChannelStatusMapper channelStatusMapper(*commandServer());
	Poco::JSON::Array::Ptr statuses = channelStatusMapper.inquire(filter);

	set("statuses", statuses);
	setView(new JSONView());
}


} } // Namespace MQ::Web
