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

#include <MQ/Web/Mapper.h>
#include <MQ/Web/Dictionary.h>

#include <MQ/CommandServer.h>

namespace MQ {
namespace Web {


class MQMapper : public Mapper
	/// Mapper for mapping objects to/from Websphere MQ
{
public:
	MQMapper(CommandServer& commandServer);


	virtual ~MQMapper();


	static void mapToJSON(const PCF& pcf, Poco::JSON::Object::Ptr& json);


	static void mapNumberToJSON(const PCF& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int parameter);


	static void mapStringToJSON(const PCF& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int parameter);


	static void mapDateToJSON(const PCF& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int dateParameter, int timeParameter);


	static std::string getReasonString(MQLONG reasonCode);


protected:

	MQ::CommandServer& _commandServer;


	static Dictionary _dictionary;


	static DisplayMap _reasonCodes;
};


}} // Namespace MQ::Web

#endif // _MQWeb_MQMapper_H
