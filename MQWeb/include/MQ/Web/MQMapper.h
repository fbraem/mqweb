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
#ifndef _MQWeb_MQMapper_H
#define _MQWeb_MQMapper_H

#include "MQ/Web/Mapper.h"
#include "MQ/Web/Dictionary.h"
#include "MQ/Web/DictionaryCache.h"

#include "MQ/CommandServer.h"

namespace MQ {
namespace Web {


class MQMapper : public Mapper
	/// Mapper for mapping objects to/from Websphere MQ
	/// It uses the Dictionary class for holding all Websphere MQ fields/values.
{
public:
	MQMapper(CommandServer& commandServer, const std::string& objectType, Poco::JSON::Object::Ptr input);
		/// Constructor

	virtual ~MQMapper();
		/// Destructor

	static Poco::JSON::Object::Ptr createError(const PCF& pcf);
		/// Returns a JSON error object based on completion code
		/// and reason code

	//const Poco::SharedPtr<Dictionary> dictionary() const;
		/// Returns the dictionary for the associated object type

	static MQLONG getOperator(const std::string& op);
		/// Returns the string equivalent for an operator

	static std::string getCommandString(MQLONG command);
		/// Translates a command code into a string

	static std::string getReasonString(MQLONG reasonCode);
		/// Translates a reason code into a string

	static const TextMap& getTextMap(const std::string& objectType, MQLONG id);
		/// Returns the TextMap for the given Websphere MQ id

	static const Poco::SharedPtr<Dictionary> dictionary(const std::string& objectType);
		/// Returns the dictionary for the given object type

protected:

	void createCommand(MQLONG command);

	Poco::JSON::Object::Ptr createJSON(const PCF& pcf);

	void addAttributeList(MQLONG attrId, const std::string& attr);
		/// Handles the attribute list

	void addIntegerFilter();
		/// When IntegerFilterCommand is set, it will add an integer filter to the PCF message

	template<typename T>
	void addParameter(MQLONG parameter, const std::string& name);

	void addParameterNumFromString(MQLONG parameter, const std::string& name);

	void addStringFilter();
		/// When StringFilterCommand is set, it will add an string filter to the PCF message

	void execute(PCF::Vector& response);
		/// Sends the PCF command to the command server

	PCF::Ptr pcf();

	Poco::JSON::Object::Ptr _input;

private:

	CommandServer& _commandServer;

	Poco::SharedPtr<Dictionary> _dictionary;

	static DictionaryCache _dictionaryCache;

	PCF::Ptr _pcf;

	static Dictionary _operators;
};

template<typename T>
inline void MQMapper::addParameter(MQLONG parameter, const std::string& name)
{
	poco_assert_dbg(!_pcf.isNull());

	Poco::Dynamic::Var value = _input->get(name);
	if (! value.isEmpty() )
	{
		try
		{
			_pcf->addParameter(parameter, value.convert<T>());
		}
		catch(...)
		{
			poco_assert_dbg(false);
		}
	}
}
/*
inline const Poco::SharedPtr<Dictionary> MQMapper::dictionary() const
{
	return _dictionary;
}
*/

inline Poco::JSON::Object::Ptr MQMapper::createJSON(const PCF &pcf)
{
	Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
	_dictionary->mapToJSON(pcf, json);
	return json;
}

inline const Poco::SharedPtr<Dictionary> MQMapper::dictionary(const std::string& objectType)
{
	return _dictionaryCache.getDictionary(objectType);
}

inline void MQMapper::createCommand(MQLONG command)
{
	_pcf = _commandServer.createCommand(command);
}

inline PCF::Ptr MQMapper::pcf()
{
	return _pcf;
}

}} // Namespace MQ::Web

#endif // _MQWeb_MQMapper_H
