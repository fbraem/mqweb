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
#include "MQ/PCF.h"

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
	set(id, name);
	return *this;
}


Dictionary& Dictionary::operator()(MQLONG id, const std::string& name, const TextMap& textMap)
{
	set(id, name, textMap);
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

void Dictionary::mapToJSON(const PCFParameters& parameters, Poco::JSON::Object::Ptr& json, bool alwaysCreate) const
{
	std::vector<MQLONG> ids = parameters.getIds();
	for(std::vector<MQLONG>::iterator it = ids.begin(); it != ids.end(); ++it)
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

		if ( parameters.isNumber(*it) )
		{
			MQLONG value = parameters.getNumber(*it);
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
		else if ( parameters.isString(*it) )
		{
			field->set("value", parameters.getString(*it));
		}
		else if ( parameters.isNumberList(*it) )
		{
			std::vector<MQLONG> values = parameters.getNumberList(*it);
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
		else if ( parameters.isNumber64List(*it) )
		{
			std::vector<MQINT64> values = parameters.getNumber64List(*it);
			Poco::JSON::Array::Ptr jsonValues = new Poco::JSON::Array();
			field->set("value", jsonValues);
			for(std::vector<MQINT64>::iterator vit = values.begin(); vit != values.end(); ++vit)
			{
				jsonValues->add(*vit);
			}
		}
		else if ( parameters.isStringList(*it) )
		{
			Poco::JSON::Array::Ptr jsonValues = new Poco::JSON::Array();
			field->set("value", jsonValues);

			std::vector<std::string> strings = parameters.getStringList(*it);
			for(std::vector<std::string>::iterator vit = strings.begin(); vit != strings.end(); ++vit)
			{
				jsonValues->add(*vit);
			}
		}
		else if ( parameters.isGroup(*it) )
		{
			Poco::JSON::Array::Ptr jsonGroups = new Poco::JSON::Array();
			field->set("value", jsonGroups);
			
			size_t groupCount = parameters.getGroupCount(*it);
			for(size_t i = 0; i < groupCount; ++i)
			{
				Poco::JSON::Object::Ptr jsonGroup = new Poco::JSON::Object();
				jsonGroups->add(jsonGroup);
				mapToJSON(parameters.getGroup(*it, i), jsonGroup, alwaysCreate);
			}
		}
		else
		{
			poco_assert_dbg(false);
		}
	}
}

void Dictionary::mapToPCF(Poco::JSON::Object::Ptr json, MQ::PCF &pcf) const
{
	for(Poco::JSON::Object::ConstIterator it = json->begin(); it != json->end(); ++it)
	{
		MQLONG id = getIdForName(it->first);
		if ( id == -1 ) continue; // Skip unknown attributes
		if (pcf.hasParameter(id)) continue; // Already added, so skip this now

		if ( id > MQIA_FIRST && id < MQIA_LAST ) // Integer attributes
		{
			if ( hasTextMap(id) )
			{
				MQLONG value = getIdForText(id, it->second.toString());
				if ( value != -1 )
				{
					pcf.addParameter(id, value);
				}
			}
			else
			{
				if ( it->second.isNumeric() )
				{
					MQLONG value = it->second;
					pcf.addParameter(id, value);
				}
			}
		}
		else if (id > MQCA_FIRST && id < MQCA_LAST ) // String attributes
		{
			pcf.addParameter(id, it->second.toString());
		}
		else if ( id > MQBACF_FIRST && id < MQBACF_LAST_USED ) // Byte attributes
		{
			//TODO: do we need this?
		}
	}
}

}} // Namespace MQ::Web
