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
#include "MQ/Web/MQController.h"
#include "MQ/Web/ChannelStatusController.h"
#include "MQ/Web/ChannelStatusMapper.h"
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


void ChannelStatusController::inquire()
{
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter can be a channelname and will result in inquiring
	// only that channel.
	if ( parameters.size() > 1 )
	{
		filter->set("name", parameters[1]);
	}
	else
	{
		if ( form().has("name") ) filter->set("name", form().get("name"));
		if ( form().has("channelType") ) filter->set("channelType", form().get("channelType"));
	}

	if ( form().has("instanceType") ) filter->set("instanceType", form().get("instanceType"));

	ChannelStatusMapper channelStatusMapper(*commandServer());
	Poco::JSON::Array::Ptr statuses = channelStatusMapper.inquire(filter);

	set("statuses", statuses);
	setView(new JSONView());
}


} } // Namespace MQ::Web
