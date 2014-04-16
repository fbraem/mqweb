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

	const Poco::SharedPtr<Dictionary> dictionary() const;

	static std::string getReasonString(MQLONG reasonCode);
		/// Translates a reason code into a string

	static const DisplayMap& getDisplayMap(const std::string& objectType, MQLONG id);
		/// Returns the DisplayMap for the given Websphere MQ id

	static const Poco::SharedPtr<Dictionary> dictionary(const std::string& objectType);
		/// Returns the dictionary for the given object type

protected:

	MQ::CommandServer& _commandServer;


	Poco::SharedPtr<Dictionary> _dictionary;


	static DictionaryCache _dictionaryCache;
};


inline const Poco::SharedPtr<Dictionary> MQMapper::dictionary() const
{
	return _dictionary;
}

inline const Poco::SharedPtr<Dictionary> MQMapper::dictionary(const std::string& objectType)
{
	return _dictionaryCache.getDictionary(objectType);
}

}} // Namespace MQ::Web

#endif // _MQWeb_MQMapper_H
