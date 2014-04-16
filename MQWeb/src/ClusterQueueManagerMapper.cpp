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
#include "MQ/Web/ClusterQueueManagerMapper.h"
#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"

namespace MQ {
namespace Web {


ClusterQueueManagerMapper::ClusterQueueManagerMapper(CommandServer& commandServer) : MQMapper(commandServer, "ClusterQueueManager")
{
}

ClusterQueueManagerMapper::~ClusterQueueManagerMapper()
{
}


void ClusterQueueManagerMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void ClusterQueueManagerMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ClusterQueueManagerMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ClusterQueueManagerMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr clusqmgrs = new Poco::JSON::Array();

	PCF::Ptr inquireClusterQMgr = _commandServer.createCommand(MQCMD_INQUIRE_CLUSTER_Q_MGR);

	if ( filter->has("clusterName") )
	{
		inquireClusterQMgr->addParameter(MQCA_CLUSTER_NAME, filter->optValue<std::string>("name", "*"));
	}

	if ( filter->has("channelName") )
	{
		inquireClusterQMgr->addParameter(MQCACH_CHANNEL_NAME, filter->optValue<std::string>("channelName", "*"));
	}

	if ( filter->has("clusterQmgrName") )
	{
		inquireClusterQMgr->addParameter(MQCA_CLUSTER_Q_MGR_NAME, filter->optValue<std::string>("clusterQmgrName", "*"));
	}

	PCF::Vector commandResponse;
	_commandServer.sendCommand(inquireClusterQMgr, commandResponse);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		Poco::JSON::Object::Ptr clusqmgr = new Poco::JSON::Object();
		clusqmgrs->add(clusqmgr);

		dictionary()->mapToJSON(**it, clusqmgr);
	}

	return clusqmgrs;
}

}} //  Namespace MQ::Web
