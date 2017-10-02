/*
* Copyright 2017 - KBC Group NV - Franky Braem - The MIT license
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "MQ/Web/ChannelController.h"
#include "MQ/Web/ChannelInquire.h"
#include "MQ/Web/ChannelStart.h"
#include "MQ/Web/ChannelStop.h"

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
			pcfParameters->set("ChannelName", form().get("ChannelName", "*"));
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

	ChannelInquire command(*commandServer(), pcfParameters);
	set("data", command.execute());
}

void ChannelController::start()
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

	ChannelStart command(*commandServer(), pcfParameters);
	set("data", command.execute());
}

void ChannelController::stop()
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

	ChannelStop command(*commandServer(), pcfParameters);
	set("data", command.execute());
}

} } // Namespace MQ::Web
