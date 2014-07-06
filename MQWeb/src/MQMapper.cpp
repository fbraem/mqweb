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


}} //  Namespace MQ::Web
