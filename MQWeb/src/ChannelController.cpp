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
#include "MQ/Web/MQController.h"
#include "MQ/Web/ChannelController.h"
#include "MQ/Web/ChannelMapper.h"
#include "MQ/Web/JSONView.h"

namespace MQ
{
namespace Web
{

ChannelController::ChannelController() : MQController()
{
}


ChannelController::~ChannelController()
{

}


void ChannelController::inquire()
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
		if ( parameters.size() > 2 )
		{
			filter->set("type", parameters[2]);
		}
		else
		{
			setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "ChannelType is required when channelname is part of the URI-path");
			return;
		}
	}
	else
	{
		std::string channelNameField = form().get("channelName", "*");
		filter->set("name", channelNameField.empty() ? "*" : channelNameField);
		filter->set("type", form().get("channelType", "All"));
		filter->set("excludeSystem", form().get("channelExcludeSystem", "false").compare("true") == 0);
	}

	ChannelMapper channelMapper(*commandServer());
	Poco::JSON::Array::Ptr jsonChannels = channelMapper.inquire(filter);
	set("channels", jsonChannels);
	setView(new JSONView());
}


} } // Namespace MQ::Web
