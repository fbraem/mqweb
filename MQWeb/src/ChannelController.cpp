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
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("filter") && data().isObject("filter") )
	{
		pcfParameters = data().getObject("filter");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		set("filter", pcfParameters);

		std::vector<std::string> parameters = getParameters();
		// First parameter is queuemanager
		// Second parameter can be a channelname. If this is passed
		// the query parameter ChannelName is ignored. A third parameter 
		// can be used for setting the channel type. This parameter can also
		// be set using the query parameter ChannelType.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ChannelName", parameters[1]);
		}
		else
		{
			std::string channelNameField = form().get("ChannelName", "*");
			pcfParameters->set("ChannelName", channelNameField.empty() ? "*" : channelNameField);
		}

		if ( parameters.size() > 2 )
		{
			pcfParameters->set("ChannelType", parameters[2]);
		}
		else
		{
			pcfParameters->set("ChannelType", form().get("ChannelType", "All"));
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		pcfParameters->set("ChannelAttrs", attrs);

		Poco::Net::NameValueCollection::ConstIterator itAttrs = form().find("ChannelAttrs");
		if ( itAttrs == form().end() )
		{
			itAttrs = form().find("Attrs");
		}

		for(; 
			itAttrs != form().end() && Poco::icompare(itAttrs->first, "Attrs") == 0;
			++itAttrs)
		{
			attrs->add(itAttrs->second);
		}
	}

	ChannelMapper mapper(*commandServer());
	Poco::JSON::Array::Ptr channels = mapper.inquire(pcfParameters);
	set("channels", channels);
	setView(new JSONView());
}


} } // Namespace MQ::Web
