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
#include "MQ/Web/ChannelAuthenticationRecordController.h"
#include "MQ/Web/ChannelAuthenticationRecordInquire.h"
#include "MQ/Web/ChannelAuthenticationRecordSet.h"

namespace MQ
{
namespace Web
{

ChannelAuthenticationRecordController::ChannelAuthenticationRecordController() : MQController()
{
}


ChannelAuthenticationRecordController::~ChannelAuthenticationRecordController()
{

}


void ChannelAuthenticationRecordController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("input") && data().isObject("input") )
	{
		pcfParameters = data().getObject("input");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		MQController::setData("input", pcfParameters);

		std::vector<std::string> parameters = getParameters();
		// First parameter is queuemanager
		// Second parameter can be a channelname. If this is passed
		// the query parameter ChannelName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ChannelName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			pcfParameters->set("ChannelName", form().get("ChannelName", "*"));
		}

		// Address in IBM Knowledge Center!
		if (form().has("ConnectionName"))
		{
			pcfParameters->set("ConnectionName", form().get("ConnectionName"));
		}
		// ClntUser in IBM Knowledge Center!
		if (form().has("ClientUserIdentifier"))
		{
			pcfParameters->set("ClientUserIdentifier", form().get("ClientUserIdentifier"));
		}
		if (form().has("Match"))
		{
			pcfParameters->set("Match", form().get("Match"));
		}
		if (form().has("QMName"))
		{
			pcfParameters->set("QMName", form().get("QMName"));
		}
		if (form().has("SSLCertIssuer"))
		{
			pcfParameters->set("SSLCertIssuer", form().get("SSLCertIssuer"));
		}
		if (form().has("SSLPeer"))
		{
			pcfParameters->set("SSLPeer", form().get("SSLPeer"));
		}
		if (form().has("Type"))
		{
			pcfParameters->set("Type", form().get("Type"));
		}

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ChannelAuthAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ChannelAuthAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ChannelAuthAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		handleFilterForm(pcfParameters);
	}

	ChannelAuthenticationRecordInquire command(*commandServer(), pcfParameters);
	setData("data", command.execute());
}

void ChannelAuthenticationRecordController::set()
{
	Poco::JSON::Object::Ptr pcfParameters;
	if (data().has("input") && data().isObject("input"))
	{
		pcfParameters = data().getObject("input");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();

		std::vector<std::string> parameters = getParameters();
		if (parameters.size() > 1)
		{
			pcfParameters->set("ProfileName", parameters[1]);
		}
		// Copy all query parameters to PCF, except ProfileName if it is already set on the URI
		for (Poco::Net::NameValueCollection::ConstIterator it = form().begin(); it != form().end(); ++it)
		{
			if (parameters.size() > 1 && Poco::icompare(it->first, "ProfileName") == 0) continue;
			pcfParameters->set(it->first, it->second);
		}
	}
	ChannelAuthenticationRecordSet command(*commandServer(), pcfParameters);
	command.execute();
}

} } // Namespace MQ::Web
