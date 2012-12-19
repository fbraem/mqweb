#ifndef _MQWeb_Dictionary
#define _MQWeb_Dictionary

#include <cmqc.h>

#include <map>
#include <string>

namespace MQ {
namespace Web {

typedef std::map<MQLONG, std::string> DisplayMap;

class Dictionary
{
public:
	Dictionary();


	virtual ~Dictionary();


	Dictionary& operator()(MQLONG id, const std::string& name);


	Dictionary& operator()(MQLONG id, const std::string& name, const DisplayMap& displayMap);


	const DisplayMap& getDisplayMap(MQLONG id) const;


	std::string getDisplayValue(MQLONG id, MQLONG value) const;


	MQLONG getDisplayId(MQLONG id, const std::string& value) const;


	MQLONG getId(const std::string& name) const;


	std::string getName(MQLONG id) const;


	bool hasDisplayMap(MQLONG id) const;


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

}} // Namespace MQWeb

#endif // _MQWeb_Dictionary
