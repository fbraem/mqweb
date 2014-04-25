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
#include "Poco/Logger.h"

#include "Poco/Util/Application.h"

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
		dictionary = _cache.get(name); // Check it again ...
		if ( dictionary.isNull() )
		{
			dictionary = load(name);
			_cache.add(name, dictionary);
		}
	}

	return dictionary;
}

Poco::SharedPtr<Dictionary> DictionaryCache::load(const std::string& name)
{
	Poco::SharedPtr<Dictionary> dictionary = new Dictionary();

	typedef Poco::Tuple<int, std::string, Poco::Nullable<int>, Poco::Nullable<std::string> > Attribute;
	std::vector<Attribute> attributes;

	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();
	std::string databaseName = config.getString("mq.web.db", "mqweb.db");

	try
	{
		Poco::Data::Session session(Poco::Data::SQLite::Connector::KEY, databaseName);

		int objectId = 0;
		session << "SELECT id FROM objects WHERE name == :n", into(objectId), useRef(name), now;

		session << "SELECT a.id, a.name, d.value, d.display FROM object_attributes oa INNER JOIN attributes a ON oa.attribute_id = a.id LEFT JOIN displays d ON oa.attribute_id = d.attribute_id WHERE oa.object_id = ?", use(objectId), into(attributes), now;
	}
	catch(Poco::Data::DataException& de)
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");
		logger.log(de);
		return dictionary;
	}

	if ( attributes.size() == 0)
	{
		Poco::Logger& logger = Poco::Logger::get("mq.web");
		poco_error_f1(logger, "No attributes found for objecttype %s", name);
		return dictionary;
	}

	int prevAttributeId = -1;
	DisplayMap displayMap;

	std::string attributeName;

	for(std::vector<Attribute>::iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		if ( prevAttributeId != it->get<0>() )
		{
			if ( displayMap.size() > 0 )
			{
				dictionary->set(prevAttributeId, attributeName, displayMap);
				displayMap.clear();
			}
			prevAttributeId = it->get<0>();
		}

		if ( it->get<2>().isNull() )
		{
			dictionary->set(it->get<0>(), it->get<1>());
			continue;
		}

		displayMap.insert(std::make_pair(it->get<2>(), it->get<3>()));
		attributeName = it->get<1>();
	}

	// Do the last one ...
	if ( displayMap.size() > 0 )
	{
		dictionary->set(prevAttributeId, attributeName, displayMap);
		displayMap.clear();
	}

	return dictionary;
}

} } // Namespace MQ::Web
