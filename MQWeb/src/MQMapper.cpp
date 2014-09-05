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

#include "MQ/MQException.h"

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

MQMapper::Command::Command(MQMapper* mapper, MQLONG command, Poco::JSON::Object::Ptr filter)
: _mapper(mapper)
, _filter(filter)
{
	_pcf = _mapper->_commandServer.createCommand(command);
}

MQMapper::Command::~Command()
{
}

void MQMapper::Command::addIntegerFilter()
{
	Poco::JSON::Object::Ptr integerFilter = _filter->getObject("IntegerFilterCommand");
	if ( integerFilter.isNull() )
		return;

	std::string parameterName = integerFilter->optValue<std::string>("Parameter", "");
	MQLONG parameter = _mapper->dictionary()->getId(parameterName);
	if ( parameter == -1 )
		return;

	std::string opString = integerFilter->optValue<std::string>("Operator", "EQ");
	MQLONG op = getOperator(Poco::toUpper(opString));
	if ( op == -1 ) op = MQCFOP_EQUAL;
	
	Poco::Dynamic::Var value = integerFilter->get("FilterValue");
	MQLONG filterValue;
	if ( value.isString() )
	{
		// A String is passed ... try to find the MQ integer value
		filterValue = _mapper->dictionary()->getDisplayId(parameter, value);
	}
	else if ( value.isNumeric() )
	{
		filterValue = value;
	}
	_pcf->addFilter(parameter, op, filterValue);
}

void MQMapper::Command::addStringFilter()
{
	Poco::JSON::Object::Ptr stringFilter = _filter->getObject("StringFilterCommand");
	if ( stringFilter.isNull() )
		return;

	std::string parameterName = stringFilter->optValue<std::string>("Parameter", "");
	MQLONG parameter = _mapper->dictionary()->getId(parameterName);
	if ( parameter == -1 )
		return;

	std::string opString = stringFilter->optValue<std::string>("Operator", "EQ");
	MQLONG op = getOperator(Poco::toUpper(opString));
	if ( op == -1 ) op = MQCFOP_EQUAL;
	
	std::string filterValue = stringFilter->optValue<std::string>("FilterValue", "");
	_pcf->addFilter(parameter, op, filterValue);
}

void MQMapper::Command::addAttributeList(MQLONG attrId, const std::string& attr)
{
	if ( _filter->has(attr) )
	{
		Poco::JSON::Array::Ptr attrs = _filter->getArray(attr);
		if ( !attrs.isNull() && attrs->size() > 0 )
		{
			std::vector<MQLONG> numList;
			for(Poco::JSON::Array::ValueVec::const_iterator it = attrs->begin(); it != attrs->end(); ++it)
			{
				MQLONG id = _mapper->dictionary()->getId(*it);
				if ( id != -1 ) numList.push_back(id);
			}
			if ( numList.size() > 0 ) _pcf->addParameterList(attrId, numList);
		}
	}
}

void MQMapper::Command::addParameterNumFromString(MQLONG parameter, const std::string& name)
{
	Poco::Dynamic::Var value = _filter->get(name);
	if ( ! value.isEmpty() )
	{
		try
		{
			std::string stringValue = value.convert<std::string>();
			MQLONG numValue = _mapper->dictionary()->getDisplayId(parameter, stringValue);
			poco_assert_dbg(numValue != -1);
			if ( numValue != - 1 )
			{
				_pcf->addParameter(parameter, numValue);
			}
		}
		catch(...)
		{
			poco_assert_dbg(false);
		}
	}
}

void MQMapper::Command::execute(PCF::Vector& response)
{
	poco_check_ptr(_pcf);
	_mapper->_commandServer.sendCommand(_pcf, response);

	if ( response.size() > 0 )
	{
		PCF::Vector::const_iterator it = response.begin();
		if ( (*it)->getCompletionCode() == MQCC_FAILED && (*it)->getReasonCode() > 3000 && (*it)->getReasonCode() < 4000 )
		{
			static Poco::SharedPtr<Dictionary> dict = _dictionaryCache.getDictionary("Event");
			std::string command = dict->getDisplayValue(MQIACF_COMMAND, (*it)->getCommand());
			throw MQException("PCF", command, (*it)->getCompletionCode(), (*it)->getReasonCode());
		}
	}
}

}} //  Namespace MQ::Web
