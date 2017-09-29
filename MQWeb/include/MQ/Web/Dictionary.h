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
#ifndef _MQWeb_Dictionary
#define _MQWeb_Dictionary

#include <cmqc.h>

#include <map>
#include <string>

#include "Poco/JSON/Object.h"

#include "MQ/PCF.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

typedef std::map<MQLONG, std::string> TextMap;
typedef MapInitializer<MQLONG, std::string> TextMapInitializer;

class Dictionary
	/// Class for holding all names, ids and values for Websphere MQ fields.
	/// It can be used to get the field name for a given Websphere MQ field constant
	/// or to get a String representation of a value of a field.
	/// It's used by PCFCommand to translate the values, ids, ...:
	/// getName(MQCA_ALTERATION_DATE) will return "AlterationDate",
	/// while getId("AlterationDate") will return MQCA_ALTERATION_DATE.
	/// getDisplayValue(MQIA_PLATFORM, MQPL_UNIX) will return "UNIX".
{
public:
	Dictionary();
		/// Constructor

	virtual ~Dictionary();
		/// Destructor

	Dictionary& operator()(MQLONG id, const std::string& name = "");
		/// Adds the name for the given id.

	Dictionary& operator()(MQLONG id, const std::string& name, const TextMap& textMap);
		/// Adds the name and value for the given id.

	const TextMap& getTextMap(MQLONG id) const;
		/// Returns all values for the given id.

	std::string getTextForValue(MQLONG id, MQLONG value) const;
		/// Returns the display string for the given value of the given id.
		/// For example: in PCFCommand getDisplayValue(MQIA_PLATFORM, MQPL_UNIX) will return "UNIX".

	MQLONG getIdForText(MQLONG id, const std::string& value) const;
		/// Returns the id for the given value and id.
		/// For example: in PCFCommand getDisplayId(MQIA_PLATFORM, "UNIX") will return MQPL_UNIX.

	MQLONG getIdForName(const std::string& name) const;
		/// Returns the id for the given name.
		/// For example: in PCFCommand getId("Platform") will return MQIA_PLATFORM.

	std::string getNameForId(MQLONG id) const;
		/// Returns the name for the given id
		/// For example: in PCFCommand getName(MQIA_PLATFORM) will return "Platform".

	bool hasTextMap(MQLONG id) const;
		/// Returns true when the id has a corresponding map with display values.

	void mapToJSON(const PCF& pcf, Poco::JSON::Object::Ptr& json, bool alwaysCreate = true) const;

	void mapToPCF(Poco::JSON::Object::Ptr json, PCF &pcf) const;

	std::map<MQLONG, std::string>::const_iterator begin() const;
		/// Returns the begin iterator of the id map

	std::map<MQLONG, std::string>::const_iterator end() const;
		/// Returns the end iterator of the id map

	void set(MQLONG id, const std::string& name);


	void set(MQLONG id, const std::string& name, const TextMap& textMap);

private:

	std::map<MQLONG, std::string> _idMap;

	std::map<std::string, MQLONG> _nameMap;

	std::map<MQLONG, TextMap> _textMaps;
};


inline std::string Dictionary::getNameForId(MQLONG id) const
{
	std::string name;

	std::map<MQLONG, std::string>::const_iterator it = _idMap.find(id);
	if ( it != _idMap.end() )
		name = it->second;

	return name;
}


inline MQLONG Dictionary::getIdForName(const std::string& name) const
{
	MQLONG id = -1;

	std::map<std::string, MQLONG>::const_iterator it  =_nameMap.find(name);
	if ( it != _nameMap.end() )
		id = it->second;

	return id;
}


inline const TextMap& Dictionary::getTextMap(MQLONG id) const
{
	static TextMap emptyMap;

	std::map<MQLONG, TextMap>::const_iterator it = _textMaps.find(id);
	if ( it == _textMaps.end() )
		return emptyMap;

	return it->second;
}

inline bool Dictionary::hasTextMap(MQLONG id) const
{
	return _textMaps.find(id) != _textMaps.end();
}

inline std::map<MQLONG, std::string>::const_iterator Dictionary::begin() const
{
	return _idMap.begin();
}

inline std::map<MQLONG, std::string>::const_iterator Dictionary::end() const
{
	return _idMap.end();
}

inline void Dictionary::set(MQLONG id, const std::string& name)
{
	_idMap[id] = name;
	_nameMap[name] = id;
}

inline void Dictionary::set(MQLONG id, const std::string& name, const TextMap& textMap)
{
	set(id, name);
	_textMaps[id] = textMap;
}

}} // Namespace MQWeb

#endif // _MQWeb_Dictionary
