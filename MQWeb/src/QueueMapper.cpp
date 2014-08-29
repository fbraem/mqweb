/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or ? as soon they
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
#include "MQ/Web/QueueMapper.h"
#include "MQ/Web/Dictionary.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {

QueueMapper::QueueMapper(CommandServer& commandServer) : MQMapper(commandServer, "Queue")
{
}

QueueMapper::~QueueMapper()
{
}


void QueueMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Command command(this, MQCMD_INQUIRE_Q, filter);

	// Required Parameters
	command.addParameter<std::string>(MQCA_Q_NAME, "QName");

	// Optional Parameters
	command.addParameter<std::string>(MQCA_CF_STRUC_NAME, "CFStructure");
	command.addParameter<MQLONG>(MQIACF_CLUSTER_INFO, "ClusterInfo");
	command.addParameter<std::string>(MQCA_CLUSTER_NAME, "ClusterName");
	command.addParameter<std::string>(MQCA_CLUSTER_NAMELIST, "ClusterNamelist");
	command.addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	command.addIntegerFilter();
	command.addParameter<MQLONG>(MQIA_PAGESET_ID, "PageSetId");
	command.addStringFilter();
	command.addParameterNumFromString(MQIA_Q_TYPE, "QType");
	command.addAttributeList(MQIACF_Q_ATTRS, "QAttrs");

	MQLONG usage = -1;
	if ( filter->has("Usage") )
	{
		std::string usageValue = filter->optValue<std::string>("Usage", "");
		if ( Poco::icompare(usageValue, "Transmission") == 0
			|| Poco::icompare(usageValue, "XmitQ") == 0 )
		{
			usage = MQUS_TRANSMISSION;
		}
		else if ( Poco::icompare(usageValue, "Normal") == 0 )
		{
			usage = MQUS_NORMAL;
		}
	}

	PCF::Vector commandResponse;
	command.execute(commandResponse);

	bool excludeSystem = filter->optValue("ExcludeSystem", false);
	bool excludeTemp = filter->optValue("ExcludeTemp", false);

	Poco::JSON::Array::Ptr jsonQueues = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( usage != -1 && (*it)->hasParameter(MQIA_USAGE) )
		{
			MQLONG queueUsage = (*it)->getParameterNum(MQIA_USAGE);
			if ( queueUsage != usage )
			{
				continue;
			}
		}

		std::string qName = (*it)->getParameterString(MQCA_Q_NAME);
		if ( excludeSystem
			&& qName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		if ( excludeTemp
			&& (*it)->hasParameter(MQIA_DEFINITION_TYPE)
			&& (*it)->getParameterNum(MQIA_DEFINITION_TYPE) == MQQDT_TEMPORARY_DYNAMIC )
		{
			continue;
		}

		Poco::JSON::Object::Ptr jsonQueue = new Poco::JSON::Object();
		jsonQueues->add(jsonQueue);

		dictionary()->mapToJSON(**it, jsonQueue);
	}

	return jsonQueues;
}

}} //  Namespace MQ::Web
