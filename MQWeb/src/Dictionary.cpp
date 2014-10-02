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

void Dictionary::mapToJSON(const PCF& pcf, Poco::JSON::Object::Ptr& json, bool alwaysCreate) const
{
	std::vector<MQLONG> parameters = pcf.getParameters();
	for(std::vector<MQLONG>::iterator it = parameters.begin(); it != parameters.end(); ++it)
	{
		std::string name = getName(*it);
		if ( name.empty() )
		{
			if ( alwaysCreate )
			{
				name = "id_" + Poco::NumberFormatter::format(*it);
			}
			else
			{
				continue;
			}
		}

		if ( json->has(name) ) continue; // Don't overwrite already added properties

		Poco::JSON::Object::Ptr field = new Poco::JSON::Object();
		json->set(name, field);

		field->set("id", *it);

		if ( pcf.isNumber(*it) )
		{
			MQLONG value = pcf.getParameterNum(*it);
			field->set("value", value);

			if ( hasDisplayMap(*it) )
			{
				std::string displayValue = getDisplayValue(*it, value);
				if ( displayValue.empty() )
				{
					displayValue = "Unknown value " + Poco::NumberFormatter::format(value) + " for " + Poco::NumberFormatter::format(*it);
				}
				field->set("display", displayValue);
			}
		}
		else if ( pcf.isString(*it) )
		{
			field->set("value", pcf.getParameterString(*it));
		}
		else if ( pcf.isNumberList(*it) )
		{
			std::vector<MQLONG> values = pcf.getParameterNumList(*it);
			Poco::JSON::Array::Ptr jsonValues = new Poco::JSON::Array();
			field->set("value", jsonValues);

			if ( hasDisplayMap(*it) )
			{
				for(std::vector<MQLONG>::iterator vit = values.begin(); vit != values.end(); ++vit)
				{
					Poco::JSON::Object::Ptr jsonValueObject = new Poco::JSON::Object();

					std::string displayValue = getDisplayValue(*it, *vit);
					if ( displayValue.empty() )
					{
						displayValue = "Unknown value " + Poco::NumberFormatter::format(*vit) + " for " + Poco::NumberFormatter::format(*it);
					}
					jsonValueObject->set("value", *vit);
					jsonValueObject->set("display", displayValue);
					jsonValues->add(jsonValueObject);
				}
			}
			else
			{
				for(std::vector<MQLONG>::iterator vit = values.begin(); vit != values.end(); ++vit)
				{
					jsonValues->add(*vit);
				}
			}
		}
		else if ( pcf.isStringList(*it) )
		{
			Poco::JSON::Array::Ptr jsonValues = new Poco::JSON::Array();
			field->set("value", jsonValues);

			std::vector<std::string> strings = pcf.getParameterStringList(*it);
			for(std::vector<std::string>::iterator vit = strings.begin(); vit != strings.end(); ++vit)
			{
				jsonValues->add(*vit);
			}
		}
		else
		{
			poco_assert_dbg(false);
		}
	}
}

}} // Namespace MQ::Web
