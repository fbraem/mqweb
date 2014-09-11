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
	Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();

	std::vector<std::string> parameters = getParameters();
	// First parameter is queuemanager
	// Second parameter is a clustername
	if ( parameters.size() > 1 )
	{
		filter->set("clusterName", parameters[1]);
	}
	else
	{
		std::string clusterNameField = form().get("clusterName", "*");
		filter->set("clusterName", clusterNameField.empty() ? "*" : clusterNameField);
		std::string clusterQmgrNameField = form().get("clusterQmgrName", "*");
		filter->set("clusterQmgrName", clusterQmgrNameField.empty() ? "*" : clusterQmgrNameField);
		std::string channelNameField = form().get("channelName", "*");
		filter->set("channelName", channelNameField.empty() ? "*" : channelNameField);
	}

	ClusterQueueManagerMapper mapper(*commandServer(), filter);
	set("clusqmgrs", mapper.inquire());
}


} } // Namespace MQ::Web
