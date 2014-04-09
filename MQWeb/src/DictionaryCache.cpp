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
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/SQLite/Connector.h"

#include "MQ/Web/DictionaryCache.h"


namespace MQ {
namespace Web {

using namespace Poco::Data::Keywords;

DictionaryCache::DictionaryCache() 
{
}


DictionaryCache::~DictionaryCache() 
{
}

Poco::SharedPtr<Dictionary> DictionaryCache::getDictionary(const std::string& name)
{
	Poco::SharedPtr<Dictionary> dictionary = _cache.get(name);
	if ( dictionary.isNull() ) // Load it
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		dictionary = load(name);
		_cache.add(name, dictionary);
	}

	return dictionary;
}

Poco::SharedPtr<Dictionary> DictionaryCache::load(const std::string& name)
{
	Poco::SharedPtr<Dictionary> dictionary;

	Poco::Data::Session session(Poco::Data::SQLite::Connector::KEY, "mqweb.db");

	int objectId = 0;
	session << "SELECT id FROM objects WHERE name == :n", into(objectId), useRef(name), now;

	session << "SELECT a.id, a.name, d.value, d.display FROM object_attributes oa INNER JOIN attributes a ON oa.attribute_id = a.id LEFT JOIN displays d ON oa.attribute_id = d.attribute_id", now;

	return dictionary;
}

} } // Namespace MQ::Web
