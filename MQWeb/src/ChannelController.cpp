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

		if ( parameters.size() > 2 )
		{
			pcfParameters->set("ChannelType", parameters[2]);
		}
		else if ( form().has("ChannelType") )
		{
			pcfParameters->set("ChannelType", form().get("ChannelType", "All"));
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ChannelAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ChannelAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ChannelAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("QSGDisposition") )
		{
			pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
		}

		if ( form().has("DefaultChannelDisposition") )
		{
			pcfParameters->set("DefaultChannelDisposition", form().get("DefaultChannelDisposition"));
		}

		handleFilterForm(pcfParameters);
	}

	ChannelMapper mapper(*commandServer(), pcfParameters);
	set("channels", mapper.inquire());
}

void ChannelController::start()
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
			// Second parameter is a channelname
			if ( parameters.size() > 1 )
			{
				  pcfParameters->set("ChannelName", parameters[1]);
			}
			else
			{
				  if ( form().has("ChannelName") ) pcfParameters->set("ChannelName", form().get("ChannelName"));
			}
 
			if ( form().has("CommandScope") ) pcfParameters->set("CommandScope", form().get("CommandScope"));
			if ( form().has("ChannelDisposition") ) pcfParameters->set("ChannelDisposition", form().get("ChannelDisposition"));
	  }
 
	  ChannelMapper mapper(*commandServer(), pcfParameters);
 
	  Poco::JSON::Object::Ptr error = mapper.start();
	  if ( error->size() > 0 ) set("error", error);
}
 
void ChannelController::stop()
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
		// Second parameter is a channelname
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ChannelName", parameters[1]);
		}
		else
		{
			if ( form().has("ChannelName") ) pcfParameters->set("ChannelName", form().get("ChannelName"));
		}
 
		if ( form().has("ChannelDisposition") ) pcfParameters->set("ChannelDisposition", form().get("ChannelDisposition"));
		if ( form().has("ChannelStatus") ) pcfParameters->set("ChannelStatus", form().get("ChannelStatus"));
		if ( form().has("ConnectionName") ) pcfParameters->set("ConnectionName", form().get("ConnectionName"));
		if ( form().has("Mode") ) pcfParameters->set("Mode", form().get("Mode"));
		if ( form().has("QMgrName") ) pcfParameters->set("QMgrName", form().get("QMgrName"));
	  }
 
	  ChannelMapper mapper(*commandServer(), pcfParameters);
 
	  Poco::JSON::Object::Ptr error = mapper.stop();
	  if ( error->size() > 0 ) set("error", error);
}

} } // Namespace MQ::Web
