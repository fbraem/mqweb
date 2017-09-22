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
	_idMap.insert(std::make_pair(id, name));
	_nameMap.insert(std::make_pair(name, id));

	return *this;
}


Dictionary& Dictionary::operator()(MQLONG id, const std::string& name, const TextMap& textMap)
{
	_idMap.insert(std::make_pair(id, name));
	_nameMap.insert(std::make_pair(name, id));

	_textMaps.insert(std::make_pair(id, textMap));

	return *this;
}

std::string Dictionary::getTextForValue(MQLONG id, MQLONG value) const
{
	std::map<MQLONG, TextMap>::const_iterator it = _textMaps.find(id);
	if ( it == _textMaps.end() )
		return "";

	TextMap::const_iterator it2 = it->second.find(value);
	if ( it2 == it->second.end() )
		return "";

	return it2->second;
}

MQLONG Dictionary::getIdForText(MQLONG id, const std::string& value) const
{
	std::map<MQLONG, TextMap>::const_iterator it = _textMaps.find(id);
	if ( it == _textMaps.end() )
		return -1;

	for(TextMap::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
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
		std::string name = getNameForId(*it);
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

			if ( hasTextMap(*it) )
			{
				std::string text = getTextForValue(*it, value);
				if ( text.empty() )
				{
					text = "Unknown value " + Poco::NumberFormatter::format(value) + " for " + Poco::NumberFormatter::format(*it);
				}
				field->set("text", text);
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

			if ( hasTextMap(*it) )
			{
				for(std::vector<MQLONG>::iterator vit = values.begin(); vit != values.end(); ++vit)
				{
					Poco::JSON::Object::Ptr jsonValueObject = new Poco::JSON::Object();

					std::string text = getTextForValue(*it, *vit);
					if ( text.empty() )
					{
						text = "Unknown value " + Poco::NumberFormatter::format(*vit) + " for " + Poco::NumberFormatter::format(*it);
					}
					jsonValueObject->set("value", *vit);
					jsonValueObject->set("text", text);
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
