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

namespace MQ {
namespace Web {

QueueMapper::QueueMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "Queue", input)
{
}

QueueMapper::~QueueMapper()
{
}


void QueueMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueMapper::create(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_Q);

	// Required Parameters
	addParameter<std::string>(MQCA_Q_NAME, "QName");

	// Optional Parameters
	addParameter<std::string>(MQCA_CF_STRUC_NAME, "CFStructure");

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
	addParameter<std::string>(MQCA_CLUSTER_NAME, "ClusterName");
	addParameter<std::string>(MQCA_CLUSTER_NAMELIST, "ClusterNamelist");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addParameter<MQLONG>(MQIA_PAGESET_ID, "PageSetId");
	addStringFilter();
	addParameterNumFromString(MQIA_Q_TYPE, "QType");
	addAttributeList(MQIACF_Q_ATTRS, "QAttrs");
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");

	MQLONG usage = -1;
	if ( _input->has("Usage") )
	{
		std::string usageValue = _input->optValue<std::string>("Usage", "");
		if (   Poco::icompare(usageValue, "Transmission") == 0
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
	execute(commandResponse);

	bool excludeSystem = _input->optValue("ExcludeSystem", false);
	bool excludeTemp = _input->optValue("ExcludeTemp", false);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
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

		Poco::JSON::Object::Ptr data = new Poco::JSON::Object();
		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
