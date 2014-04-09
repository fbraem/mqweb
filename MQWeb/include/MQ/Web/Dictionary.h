#ifndef _MQWeb_Dictionary
#define _MQWeb_Dictionary

#include <cmqc.h>

#include <map>
#include <string>

#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

typedef std::map<MQLONG, std::string> DisplayMap;
typedef MapInitializer<MQLONG, std::string> DisplayMapInitializer;

class Dictionary
	/// Class for holding all names, ids and values for Websphere MQ fields.
	/// It can be used to get the field name for a given Websphere MQ field constant
	/// or to get a String representation of a value of a field.
	/// It's used by MQMapper to translate the values, ids, ...:
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

	Dictionary& operator()(MQLONG id, const std::string& name, const DisplayMap& displayMap);
		/// Adds the name and value for the given id.

	const DisplayMap& getDisplayMap(MQLONG id) const;
		/// Returns all values for the given id.

	std::string getDisplayValue(MQLONG id, MQLONG value) const;
		/// Returns the display string for the given value of the given id.
		/// For example: in MQMapper getDisplayValue(MQIA_PLATFORM, MQPL_UNIX) will return "UNIX".

	MQLONG getDisplayId(MQLONG id, const std::string& value) const;
		/// Returns the id for the given value and id.
		/// For example: in MQMapper getDisplayId(MQIA_PLATFORM, "UNIX") will return MQPL_UNIX.

	MQLONG getId(const std::string& name) const;
		/// Returns the id for the given name.
		/// For example: in MQMapper getId("Platform") will return MQIA_PLATFORM.

	std::string getName(MQLONG id) const;
		/// Returns the name for the given id
		/// For example: in MQMapper getName(MQIA_PLATFORM) will return "Platform".

	bool hasDisplayMap(MQLONG id) const;
		/// Returns true when the id has a corresponding map with display values.

	std::map<MQLONG, std::string>::const_iterator begin() const;
		/// Returns the begin iterator of the id map

	std::map<MQLONG, std::string>::const_iterator end() const;
		/// Returns the end iterator of the id map

	void set(MQLONG id, const std::string& name);

private:

	std::map<MQLONG, std::string> _idMap;

	std::map<std::string, MQLONG> _nameMap;

	std::map<MQLONG, DisplayMap> _displayMaps;
};


inline std::string Dictionary::getName(MQLONG id) const
{
	std::string name;

	std::map<MQLONG, std::string>::const_iterator it = _idMap.find(id);
	if ( it != _idMap.end() )
		name = it->second;

	return name;
}


inline MQLONG Dictionary::getId(const std::string& name) const
{
	MQLONG id = -1;

	std::map<std::string, MQLONG>::const_iterator it  =_nameMap.find(name);
	if ( it != _nameMap.end() )
		id = it->second;

	return id;
}


inline const DisplayMap& Dictionary::getDisplayMap(MQLONG id) const
{
	static DisplayMap emptyMap;

	std::map<MQLONG, DisplayMap>::const_iterator it = _displayMaps.find(id);
	if ( it == _displayMaps.end() )
		return emptyMap;

	return it->second;
}

inline bool Dictionary::hasDisplayMap(MQLONG id) const
{
	return _displayMaps.find(id) != _displayMaps.end();
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
}


}} // Namespace MQWeb

#endif // _MQWeb_Dictionary
