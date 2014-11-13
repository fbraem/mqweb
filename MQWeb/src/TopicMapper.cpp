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
#include "MQ/Web/TopicMapper.h"

namespace MQ {
namespace Web {


TopicMapper::TopicMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input) 
: MQMapper(commandServer, "Topic", input)
{
}

TopicMapper::~TopicMapper()
{
}


void TopicMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void TopicMapper::create(bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void TopicMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr TopicMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_TOPIC);

	// Required parameters
	addParameter<std::string>(MQCA_PROCESS_NAME, "TopicName");

	// Optional parameters
	Poco::Dynamic::Var clusterInfo = _input->get("ClusterInfo");
	if (! clusterInfo.isEmpty() )
	{
		try
		{
			pcf()->addParameter(MQIACF_CLUSTER_INFO, clusterInfo.convert<bool>() == true);
		}
		catch(...)
		{
			poco_assert_dbg(false);
		}
	}

	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	addStringFilter();
	addAttributeList(MQIACF_PROCESS_ATTRS, "TopicAttrs");
	addParameterNumFromString(MQIA_TOPIC_TYPE, "TopicType");

	PCF::Vector commandResponse;
	execute(commandResponse);

	bool excludeSystem = _input->optValue("ExcludeSystem", false);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		std::string processName = (*it)->getParameterString(MQCA_PROCESS_NAME);
		if (   excludeSystem
			&& processName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
