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
#include "MQ/Web/ClusterQueueManagerController.h"
#include "MQ/Web/ClusterQueueManagerInquire.h"
#include "MQ/Web/ClusterQueueManagerSuspend.h"
#include "MQ/Web/ClusterQueueManagerResume.h"

namespace MQ
{
namespace Web
{

ClusterQueueManagerController::ClusterQueueManagerController() : MQController()
{
}


ClusterQueueManagerController::~ClusterQueueManagerController()
{

}


void ClusterQueueManagerController::inquire()
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
		// Second parameter is a clustername
		// Third parameter is a cluster queuemanager
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ClusterName", parameters[1]);
		}
		else
		{
			std::string clusterNameField;
			if ( form().has("ClusterName") )
			{
				clusterNameField = form().get("ClusterName");
			}
			else if ( form().has("name") )
			{
				clusterNameField = form().get("name");
			}
			if ( !clusterNameField.empty() )
			{
				pcfParameters->set("ClusterName", clusterNameField);
			}
		}

		if ( parameters.size() > 2 )
		{
			pcfParameters->set("ClusterQMgrName", parameters[2]);
		}
		else
		{
			std::string clusterQmgrNameField = form().get("ClusterQMgrName", "*");
			pcfParameters->set("ClusterQMgrName", clusterQmgrNameField.empty() ? "*" : clusterQmgrNameField);
		}

		if ( form().has("ChannelName") ) pcfParameters->set("ChannelName", form().get("ChannelName"));

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("ClusterQMgrAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for ClusterQMgrAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("ClusterQMgrAttrs", attrs);
		}

		if ( form().has("CommandScope") ) pcfParameters->set("CommandScope", form().get("CommandScope"));

		handleFilterForm(pcfParameters);
	}

	ClusterQueueManagerInquire command(*commandServer(), pcfParameters);
	set("data", command.execute());
}


void ClusterQueueManagerController::suspend()
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
		// Second parameter is a clustername
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ClusterName", parameters[1]);
		}
		else
		{
			if ( form().has("ClusterName") ) pcfParameters->set("ClusterName", form().get("ClusterName"));
		}

		if ( form().has("CommandScope") ) pcfParameters->set("CommandScope", form().get("CommandScope"));
		if ( form().has("Mode") ) pcfParameters->set("Mode", form().get("Mode"));
	}

	ClusterQueueManagerSuspend command(*commandServer(), pcfParameters);
	set("data", command.execute());
}


void ClusterQueueManagerController::resume()
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
		// Second parameter is a clustername
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("ClusterName", parameters[1]);
		}
		else
		{
			if ( form().has("ClusterName") ) pcfParameters->set("ClusterName", form().get("ClusterName"));
		}

		if ( form().has("CommandScope") ) pcfParameters->set("CommandScope", form().get("CommandScope"));
	}

	ClusterQueueManagerResume command(*commandServer(), pcfParameters);
	set("data", command.execute());
}


} } // Namespace MQ::Web
