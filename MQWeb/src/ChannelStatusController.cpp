/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
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
#include "MQ/Web/ChannelStatusController.h"
#include "MQ/Web/ChannelStatusMapper.h"

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
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("input") && data().isObject("input") )
	{
		pcfParameters = data().getObject("input");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		set("input", pcfParameters);

		std::vector<std::string> parameters = getParameters();
		// First parameter is queuemanager
		// Second parameter can be a channelname and will result in inquiring
		// only that channel.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ChannelName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			std::string channelNameField;
			if ( form().has("ChannelName") )
			{
				channelNameField = form().get("ChannelName");
			}
			else if ( form().has("name") )
			{
				channelNameField = form().get("name");
			}
			if ( channelNameField.empty() )
			{
				channelNameField = "*";
			}
			pcfParameters->set("ChannelName", channelNameField);
		}

		if ( form().has("ChannelType") )
		{
			Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();
			filter->set("Parameter", "ChannelType");
			filter->set("Operator", "EQ");
			filter->set("FilterValue", form().get("ChannelType"));
			pcfParameters->set("IntegerFilterCommand", filter);
		};

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ChannelInstanceAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ChannelInstanceAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ChannelInstanceAttrs", attrs);
		}

		if ( form().has("InstanceType") ) pcfParameters->set("InstanceType", form().get("InstanceType"));
		if ( form().has("ClientIdentifier") ) pcfParameters->set("ClientIdentifier", form().get("ClientIdentifier"));
		if ( form().has("ChannelInstanceType") ) pcfParameters->set("ChannelInstanceType", form().get("ChannelInstanceType"));
		if ( form().has("CommandScope") ) pcfParameters->set("CommandScope", form().get("CommandScope"));
		if ( form().has("ConnectionName") ) pcfParameters->set("ConnectionName", form().get("ConnectionName"));
		if ( form().has("XmitQName") ) pcfParameters->set("XmitQName", form().get("XmitQName"));

		handleFilterForm(pcfParameters);
	}
	ChannelStatusMapper mapper(*commandServer(), pcfParameters);
	set("statuses", mapper.inquire());
}


} } // Namespace MQ::Web
