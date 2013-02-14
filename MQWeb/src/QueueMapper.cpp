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
#include <MQ/Web/QueueMapper.h>
#include <MQ/Web/Dictionary.h>
#include <MQ/MQException.h>

namespace MQ {
namespace Web {

QueueMapper::QueueMapper(CommandServer& commandServer) : MQMapper(commandServer)
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

	Poco::JSON::Array::Ptr jsonQueues = new Poco::JSON::Array();

	PCF::Ptr inquireQ = _commandServer.createCommand(MQCMD_INQUIRE_Q);
	inquireQ->addParameter(MQCA_Q_NAME, filter->optValue<std::string>("name", "*"));
	if ( filter->has("qdepth") )
	{
		inquireQ->addFilter(MQIA_CURRENT_Q_DEPTH, MQCFOP_NOT_LESS, filter->getValue<int>("qdepth"));
	}

	MQLONG usage = -1;
	if ( filter->has("usage") )
	{
		std::string usageValue = filter->optValue<std::string>("usage", "");
		if ( usageValue.compare("xmitq") == 0 )
		{
			usage = MQUS_TRANSMISSION;
		}
		else if ( usageValue.compare("normal") == 0 )
		{
			usage = MQUS_NORMAL;
		}
	}

	std::string queueType = filter->optValue<std::string>("type", "All");
	MQLONG queueTypeValue = _dictionary.getDisplayId(MQIA_Q_TYPE, queueType);
	poco_assert_dbg(queueTypeValue != -1);
	if ( queueTypeValue == - 1 )
	{
		return jsonQueues;
	}
	inquireQ->addParameter(MQIA_Q_TYPE, queueTypeValue);

	PCF::Vector commandResponse;
	_commandServer.sendCommand(inquireQ, commandResponse);
	if ( commandResponse.size() > 0 )
	{
		PCF::Vector::iterator it = commandResponse.begin();
		if ( (*it)->getCompletionCode() != MQCC_OK )
		{
			if ( (*it)->getReasonCode() != MQRC_UNKNOWN_OBJECT_NAME )
			{
				throw MQException(_commandServer.qmgr().name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
			}
		}

		bool excludeSystem = filter->optValue("excludeSystem", false);
		bool excludeTemp = filter->optValue("excludeTemp", false);

		for(; it != commandResponse.end(); it++)
		{
			if ( (*it)->isExtendedResponse() ) // Skip extended response
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
				&& (*it)->getParameterNum(MQIA_DEFINITION_TYPE) == MQQDT_TEMPORARY_DYNAMIC )
			{
				continue;
			}

			Poco::JSON::Object::Ptr jsonQueue = new Poco::JSON::Object();
			jsonQueues->add(jsonQueue);

			mapToJSON(**it, jsonQueue);
		}
	}

	return jsonQueues;
}

}} //  Namespace MQ::Web
