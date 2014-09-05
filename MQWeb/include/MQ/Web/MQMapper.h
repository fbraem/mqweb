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
	MQMapper(CommandServer& commandServer, const std::string& objectType);
		/// Constructor

	virtual ~MQMapper();
		/// Destructor

	const CommandServer& commandServer() const;
		/// Returns the associated commandserver

	const Poco::SharedPtr<Dictionary> dictionary() const;
		/// Returns the dictionary for the associated object type

	static MQLONG getOperator(const std::string& op);
		/// Returns the string equivalent for an operator

	static std::string getReasonString(MQLONG reasonCode);
		/// Translates a reason code into a string

	static const DisplayMap& getDisplayMap(const std::string& objectType, MQLONG id);
		/// Returns the DisplayMap for the given Websphere MQ id

	static const Poco::SharedPtr<Dictionary> dictionary(const std::string& objectType);
		/// Returns the dictionary for the given object type

protected:

	class Command
	{
	public:
		Command(MQMapper* mapper, MQLONG command, Poco::JSON::Object::Ptr filter);

		virtual ~Command();

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
			/// Returns the PCF command message

	private:

		MQMapper* _mapper;

		PCF::Ptr _pcf;

		Poco::JSON::Object::Ptr _filter;
	};

private:

	CommandServer& _commandServer;


	Poco::SharedPtr<Dictionary> _dictionary;


	static DictionaryCache _dictionaryCache;
};

template<typename T>
inline void MQMapper::Command::addParameter(MQLONG parameter, const std::string& name)
{
	Poco::Dynamic::Var value = _filter->get(name);
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

inline const CommandServer& MQMapper::commandServer() const
{
	return _commandServer;
}

inline const Poco::SharedPtr<Dictionary> MQMapper::dictionary() const
{
	return _dictionary;
}

inline const Poco::SharedPtr<Dictionary> MQMapper::dictionary(const std::string& objectType)
{
	return _dictionaryCache.getDictionary(objectType);
}

inline PCF::Ptr MQMapper::Command::pcf()
{
	return _pcf;
}

}} // Namespace MQ::Web

#endif // _MQWeb_MQMapper_H
