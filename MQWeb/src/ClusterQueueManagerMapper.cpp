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
#include "MQ/Web/ClusterQueueManagerMapper.h"

namespace MQ {
namespace Web {


ClusterQueueManagerMapper::ClusterQueueManagerMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "ClusterQueueManager", input)
{
}

ClusterQueueManagerMapper::~ClusterQueueManagerMapper()
{
}


void ClusterQueueManagerMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void ClusterQueueManagerMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ClusterQueueManagerMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ClusterQueueManagerMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_CLUSTER_Q_MGR);

	// Required parameters
	addParameter<std::string>(MQCA_CLUSTER_Q_MGR_NAME, "ClusterQmgrName");

	// Optional parameters
	addParameter<std::string>(MQCACH_CHANNEL_NAME, "ChannelName");
	addParameter<std::string>(MQCA_CLUSTER_NAME, "ClusterName");
	addAttributeList(MQIACF_CLUSTER_Q_MGR_ATTRS, "ClusterQMgrAttrs");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addStringFilter();

	PCF::Vector commandResponse;
	execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
