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
#include "MQ/Web/ClusterQueueManagerController.h"
#include "MQ/Web/ClusterQueueManagerMapper.h"

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
		// Second parameter is a clustername
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

		std::string clusterQmgrNameField = form().get("ClusterQmgrName", "*");
		pcfParameters->set("ClusterQmgrName", clusterQmgrNameField.empty() ? "*" : clusterQmgrNameField);

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

	ClusterQueueManagerMapper mapper(*commandServer(), pcfParameters);
	set("clusqmgrs", mapper.inquire());
}


} } // Namespace MQ::Web
