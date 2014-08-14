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
#include "MQ/Web/MQMapper.h"

#include "Poco/JSON/Object.h"
#include "Poco/Logger.h"

namespace MQ {
namespace Web {

DictionaryCache MQMapper::_dictionaryCache;


MQMapper::MQMapper(CommandServer& commandServer, const std::string& objectType) : _commandServer(commandServer)
{
	_dictionary = _dictionaryCache.getDictionary(objectType);
	poco_assert_dbg(!_dictionary.isNull());

	if ( _dictionary.isNull() )
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");
		logger.error(Poco::Logger::format("$0 not found in dictionary database", objectType));
	}
}

MQMapper::~MQMapper()
{
}


std::string MQMapper::getReasonString(MQLONG reasonCode)
{
	static Poco::SharedPtr<Dictionary> dict = _dictionaryCache.getDictionary("Reason");

	return dict->getDisplayValue(MQIACF_REASON_CODE, reasonCode);
}

MQLONG MQMapper::getOperator(const std::string& op)
{
	static Dictionary operators = Dictionary()
		(MQCFOP_LESS, "LT")
		(MQCFOP_LESS | MQCFOP_EQUAL, "LE")
		(MQCFOP_EQUAL, "EQ")
		(MQCFOP_GREATER, "GT")
		(MQCFOP_GREATER | MQCFOP_EQUAL, "GE")
		(MQCFOP_NOT_LESS, "NLT")
		(MQCFOP_NOT_EQUAL, "NE")
		(MQCFOP_NOT_GREATER, "NGT")
		(MQCFOP_LIKE, "LK")
		(MQCFOP_NOT_LIKE, "NL")
		(MQCFOP_CONTAINS, "CT")
		(MQCFOP_EXCLUDES, "EX")
		(MQCFOP_CONTAINS_GEN, "CTG")
		(MQCFOP_EXCLUDES_GEN, "EXG")
	;
	return operators.getId(op);
}

const DisplayMap& MQMapper::getDisplayMap(const std::string& objectType, MQLONG id)
{
	Poco::SharedPtr<Dictionary> dict = _dictionaryCache.getDictionary(objectType);
	poco_assert_dbg(!dict.isNull());

	if ( dict.isNull() )
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");
		logger.error(Poco::Logger::format("$0 not found in dictionary database", objectType));
	}

	return dict->getDisplayMap(id);
}

void MQMapper::handleIntegerFilter(PCF::Ptr pcf, Poco::JSON::Object::Ptr filter)
{
	if ( filter->has("IntegerFilterCommand") )
	{
		Poco::JSON::Object::Ptr integerFilterCommand = filter->getObject("IntegerFilterCommand");
		if ( !integerFilterCommand.isNull() )
		{
			std::string parameterName = integerFilterCommand->optValue<std::string>("Parameter", "");
			MQLONG parameter = dictionary()->getId(parameterName);
			if ( parameter != -1 )
			{
				std::string opString = integerFilterCommand->optValue<std::string>("Operator", "EQ");
				MQLONG op = getOperator(Poco::toUpper(opString));
				if ( op == -1 ) op = MQCFOP_EQUAL;
				
				Poco::Dynamic::Var value = integerFilterCommand->get("FilterValue");
				MQLONG filterValue;
				if ( value.isString() )
				{
					// A String is passed ... try to find the MQ integer value
					filterValue = dictionary()->getDisplayId(parameter, value);
				}
				else if ( value.isNumeric() )
				{
					filterValue = value;
				}
				pcf->addFilter(parameter, op, filterValue);
			}
		}
	}
}

void MQMapper::handleStringFilter(PCF::Ptr pcf, Poco::JSON::Object::Ptr filter)
{
	if ( filter->has("StringFilterCommand") )
	{
		Poco::JSON::Object::Ptr integerFilterCommand = filter->getObject("StringFilterCommand");
		if ( !integerFilterCommand.isNull() )
		{
			std::string parameterName = integerFilterCommand->optValue<std::string>("Parameter", "");
			MQLONG parameter = dictionary()->getId(parameterName);
			if ( parameter != -1 )
			{
				std::string opString = integerFilterCommand->optValue<std::string>("Operator", "EQ");
				MQLONG op = getOperator(Poco::toUpper(opString));
				if ( op == -1 ) op = MQCFOP_EQUAL;
				
				std::string filterValue = integerFilterCommand->optValue<std::string>("FilterValue", "");
				pcf->addFilter(parameter, op, filterValue);
			}
		}
	}
}

}} //  Namespace MQ::Web
