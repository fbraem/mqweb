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

#include "MQ/Web/Dictionary.h"

namespace MQ {
namespace Web {

Dictionary::Dictionary()
{
}


Dictionary::~Dictionary()
{
}


Dictionary& Dictionary::operator()(MQLONG id, const std::string& name)
{
	_idMap.insert(std::make_pair<MQLONG, std::string>(id, name));
	_nameMap.insert(std::make_pair<std::string, MQLONG>(name, id));

	return *this;
}


Dictionary& Dictionary::operator()(MQLONG id, const std::string& name, const DisplayMap& displayMap)
{
	_idMap.insert(std::make_pair<MQLONG, std::string>(id, name));
	_nameMap.insert(std::make_pair<std::string, MQLONG>(name, id));

	_displayMaps.insert(std::make_pair<MQLONG, DisplayMap>(id, displayMap));

	return *this;
}

std::string Dictionary::getDisplayValue(MQLONG id, MQLONG displayId) const
{
	std::map<MQLONG, DisplayMap>::const_iterator it = _displayMaps.find(id);
	if ( it == _displayMaps.end() )
		return "";

	DisplayMap::const_iterator it2 = it->second.find(displayId);
	if ( it2 == it->second.end() )
		return "";

	return it2->second;
}

MQLONG Dictionary::getDisplayId(MQLONG id, const std::string& value) const
{
	std::map<MQLONG, DisplayMap>::const_iterator it = _displayMaps.find(id);
	if ( it == _displayMaps.end() )
		return -1;

	for(DisplayMap::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
	{
		if ( it2->second.compare(value) == 0 )
			return it2->first;
	}
	return -1;
}

}} // Namespace MQ::Web
