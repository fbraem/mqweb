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
#ifndef _MQWeb_PCFCommand_H
#define _MQWeb_PCFCommand_H

#include "MQ/Web/Mapper.h"
#include "MQ/Web/Dictionary.h"
#include "MQ/Web/DictionaryCache.h"

#include "MQ/CommandServer.h"

namespace MQ {
namespace Web {


class PCFCommand
	/// Mapper for mapping objects to/from Websphere MQ
	/// It uses the Dictionary class for holding all Websphere MQ fields/values.
{
public:
	PCFCommand(CommandServer& commandServer, MQLONG command, const std::string& objectType, Poco::JSON::Object::Ptr input);
		/// Constructor

	virtual ~PCFCommand();
		/// Destructor

	//static Poco::JSON::Object::Ptr createError(const PCF& pcf);
		/// Returns a JSON error object based on completion code
		/// and reason code

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

	void execute();
		/// Sends the PCF command to the command server

	PCF::Vector::const_iterator begin() const;

	PCF::Vector::const_iterator end() const;

	size_t responseSize() const;

protected:

	PCF::Ptr createCommand(MQLONG command);

	Poco::JSON::Object::Ptr createJSON(const PCF& pcf);

	void fillPCF();

	void addAttributeList(MQLONG attrId, const std::string& attr);
		/// Handles the attribute list

	void addIntegerFilter();
		/// When IntegerFilterCommand is set, it will add an integer filter to the PCF message

	template<typename T>
	void addParameter(MQLONG parameter, const std::string& name);

	void addParameterNumFromString(MQLONG parameter, const std::string& name);

	void addStringFilter();
		/// When StringFilterCommand is set, it will add an string filter to the PCF message

	PCF::Ptr pcf();

private:

	CommandServer& _commandServer;

	Poco::JSON::Object::Ptr _input;

	PCF::Ptr _pcf;

	PCF::Vector _response;

	Poco::SharedPtr<Dictionary> _dictionary;

	static Dictionary _operators;
};

template<typename T>
inline void PCFCommand::addParameter(MQLONG parameter, const std::string& name)
{
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

inline Poco::JSON::Object::Ptr PCFCommand::createJSON(const PCF &pcf)
{
	Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
	_dictionary->mapToJSON(pcf, json);
	return json;
}

inline const Poco::SharedPtr<Dictionary> PCFCommand::dictionary(const std::string& objectType)
{
	return DictionaryCache::instance()->getDictionary(objectType);
}

inline PCF::Ptr PCFCommand::createCommand(MQLONG command)
{
	return _commandServer.createCommand(command);
}

inline void PCFCommand::fillPCF()
{
	_dictionary->mapToPCF(_input, *_pcf);
}

inline PCF::Vector::const_iterator PCFCommand::begin() const
{
	return _response.begin();
}

inline PCF::Vector::const_iterator PCFCommand::end() const
{
	return _response.end();
}

inline size_t PCFCommand::responseSize() const
{
	return _response.size();
}

inline PCF::Ptr PCFCommand::pcf()
{
	return _pcf;
}

}} // Namespace MQ::Web

#endif // _MQWeb_PCFCommand_H
