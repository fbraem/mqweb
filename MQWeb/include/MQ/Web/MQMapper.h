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
