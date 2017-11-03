/*
* Copyright 2017 - KBC Group NV - Franky Braem - The MIT license
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "MQ/Web/PCFCommand.h"

#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"
#include "Poco/Logger.h"

namespace MQ {
namespace Web {

Dictionary PCFCommand::_operators = Dictionary()
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

PCFCommand::PCFCommand(CommandServer& commandServer, MQLONG command, const std::string& objectType, Poco::JSON::Object::Ptr input)
  : _commandServer(commandServer)
  , _input(input)
{
	_pcf = createCommand(command);
	_dictionary = DictionaryCache::instance()->getDictionary(objectType);
	poco_assert_dbg(!_dictionary.isNull());

	if ( _dictionary.isNull() )
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");
		logger.error(Poco::Logger::format("$0 not found in dictionary database", objectType));
	}
}

PCFCommand::~PCFCommand()
{
}


std::string PCFCommand::getReasonString(MQLONG reasonCode)
{
	Poco::SharedPtr<Dictionary> dict = DictionaryCache::instance()->getDictionary("Reason");
	poco_assert_dbg(!dict.isNull());

	return dict->getTextForValue(MQIACF_REASON_CODE, reasonCode);
}

std::string PCFCommand::getCommandString(MQLONG command)
{
	Poco::SharedPtr<Dictionary> dict = DictionaryCache::instance()->getDictionary("Event");
	poco_assert_dbg(!dict.isNull());

	return dict->getTextForValue(MQIACF_COMMAND, command);
}

MQLONG PCFCommand::getOperator(const std::string& op)
{
	return _operators.getIdForName(op);
}

const TextMap& PCFCommand::getTextMap(const std::string& objectType, MQLONG id)
{
	Poco::SharedPtr<Dictionary> dict = DictionaryCache::instance()->getDictionary(objectType);
	poco_assert_dbg(!dict.isNull());

	if ( dict.isNull() )
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");
		logger.error(Poco::Logger::format("$0 not found in dictionary database", objectType));
	}

	return dict->getTextMap(id);
}

void PCFCommand::addIntegerFilter()
{
	Poco::JSON::Object::Ptr integerFilter = _input->getObject("IntegerFilterCommand");
	if ( integerFilter.isNull() )
		return;

	std::string parameterName = integerFilter->optValue<std::string>("Parameter", "");
	MQLONG parameter = _dictionary->getIdForName(parameterName);
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
		filterValue = _dictionary->getIdForText(parameter, value);
	}
	else if ( value.isNumeric() )
	{
		filterValue = value;
	}
	_pcf->addFilter(parameter, op, filterValue);
}

void PCFCommand::addStringFilter()
{
	Poco::JSON::Object::Ptr stringFilter = _input->getObject("StringFilterCommand");
	if ( stringFilter.isNull() )
		return;

	std::string parameterName = stringFilter->optValue<std::string>("Parameter", "");
	MQLONG parameter = _dictionary->getIdForName(parameterName);
	if ( parameter == -1 )
		return;

	std::string opString = stringFilter->optValue<std::string>("Operator", "EQ");
	MQLONG op = getOperator(Poco::toUpper(opString));
	if ( op == -1 ) op = MQCFOP_EQUAL;

	std::string filterValue = stringFilter->optValue<std::string>("FilterValue", "");
	_pcf->addFilter(parameter, op, filterValue);
}

void PCFCommand::addAttributeList(MQLONG attrId, const std::string& attr)
{
	if ( _input->has(attr) )
	{
		Poco::JSON::Array::Ptr attrs = _input->getArray(attr);
		if ( !attrs.isNull() && attrs->size() > 0 )
		{
			std::vector<MQLONG> numList;

			for(Poco::JSON::Array::ValueVec::const_iterator it = attrs->begin(); it != attrs->end(); ++it)
			{
				if ( Poco::icompare(it->toString(), "All") == 0 )
				{
					numList.push_back(MQIACF_ALL);
				}
				MQLONG id = _dictionary->getIdForName(*it);
				if ( id != -1 ) numList.push_back(id);
			}
			if ( numList.size() > 0 ) _pcf->addParameterList(attrId, numList);
		}
	}
}

void PCFCommand::addParameterNumFromString(MQLONG parameter, const std::string& name)
{
	Poco::Dynamic::Var value = _input->get(name);
	if ( ! value.isEmpty() )
	{
		try
		{
			std::string stringValue = value.convert<std::string>();
			MQLONG numValue = _dictionary->getIdForText(parameter, stringValue);
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

void PCFCommand::execute()
{
	_response.clear();
	_commandServer.sendCommand(_pcf, _response);
/*
	if ( _response.size() > 0 )
	{
		PCF::Vector::const_iterator it = _response.begin();
		if (     (*it)->getCompletionCode() == MQCC_FAILED
			  && (*it)->getReasonCode() > 3000
			  && (*it)->getReasonCode() < 4000
			  && (*it)->getReasonCode() != MQRCCF_NONE_FOUND
			  && (*it)->getReasonCode() != MQRCCF_CHL_STATUS_NOT_FOUND
			  && (*it)->getReasonCode() != MQRCCF_TOPIC_STRING_NOT_FOUND )
		{
			throw MQException("PCF", getCommandString((*it)->getCommand()), (*it)->getCompletionCode(), (*it)->getReasonCode());
		}
	}
*/
}
/*
Poco::JSON::Object::Ptr PCFCommand::createError(const PCF& pcf)
{
	Poco::JSON::Object::Ptr error = new Poco::JSON::Object();
	error->set("object", "PCF");
	error->set("fn", getCommandString(pcf.getCommand()));
	switch(pcf.getCompletionCode())
	{
		case MQCC_OK: error->set("code", "OK"); break;
		case MQCC_WARNING: error->set("code", "WARNING"); break;
		case MQCC_FAILED: error->set("code", "ERROR"); break;
	}

	Poco::JSON::Object::Ptr reason = new Poco::JSON::Object();
	error->set("reason", reason);
	reason->set("code", pcf.getReasonCode());
	reason->set("desc", PCFCommand::getReasonString(pcf.getReasonCode()));

	return error;
}
*/

}} //  Namespace MQ::Web
