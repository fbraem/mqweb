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
#include "Poco/Logger.h"

#include "Poco/Util/Application.h"

#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/SQLite/Connector.h"

#include "MQ/Web/DictionaryCache.h"


namespace MQ {
namespace Web {

using namespace Poco::Data::Keywords;

DictionaryCache* DictionaryCache::_instance;

DictionaryCache::DictionaryCache()
{
	setup();
}


DictionaryCache::~DictionaryCache()
{
}

void DictionaryCache::setup()
{
	poco_assert(_instance == NULL);
	_instance = this;
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
	Poco::Logger& logger = Poco::Logger::get("mq.web");
	poco_trace_f2(logger, "Trying to open SQLite database %s to load dictionary %s", databaseName, name);

	try
	{
		Poco::Data::Session session(Poco::Data::SQLite::Connector::KEY, databaseName);

		int objectId = 0;
		session << "SELECT id FROM objects WHERE name = :n", into(objectId), useRef(name), now;

		session << "SELECT a.id, a.name, d.value, d.text FROM object_attributes oa INNER JOIN attributes a ON oa.attribute_id = a.id LEFT JOIN texts d ON oa.attribute_id = d.attribute_id WHERE oa.object_id = ?", use(objectId), into(attributes), now;
	}
	catch(Poco::Data::DataException& de)
	{
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
	TextMap textMap;

	std::string attributeName;

	for(std::vector<Attribute>::iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		if ( prevAttributeId != it->get<0>() )
		{
			if ( textMap.size() > 0 )
			{
				dictionary->set(prevAttributeId, attributeName, textMap);
				textMap.clear();
			}
			prevAttributeId = it->get<0>();
		}

		if ( it->get<2>().isNull() )
		{
			dictionary->set(it->get<0>(), it->get<1>());
			continue;
		}

		textMap.insert(std::make_pair(it->get<2>(), it->get<3>()));
		attributeName = it->get<1>();
	}

	// Do the last one ...
	if ( textMap.size() > 0 )
	{
		dictionary->set(prevAttributeId, attributeName, textMap);
		textMap.clear();
	}

	return dictionary;
}

} } // Namespace MQ::Web
