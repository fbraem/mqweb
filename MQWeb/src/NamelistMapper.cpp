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
#include "MQ/Web/NamelistMapper.h"
#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"

namespace MQ {
namespace Web {


NamelistMapper::NamelistMapper(CommandServer& commandServer) : MQMapper(commandServer, "Namelist")
{
}

NamelistMapper::~NamelistMapper()
{
}


void NamelistMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void NamelistMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void NamelistMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr NamelistMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Poco::JSON::Array::Ptr namelists = new Poco::JSON::Array();

	PCF::Ptr inquireNamelist = _commandServer.createCommand(MQCMD_INQUIRE_NAMELIST);

	inquireNamelist->addParameter(MQCA_NAMELIST_NAME, filter->optValue<std::string>("name", "*"));

	PCF::Vector commandResponse;
	_commandServer.sendCommand(inquireNamelist, commandResponse);

	bool excludeSystem = filter->optValue("excludeSystem", false);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		std::string namelistName = (*it)->getParameterString(MQCA_NAMELIST_NAME);
		if (   excludeSystem
			&& namelistName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		Poco::JSON::Object::Ptr namelist = new Poco::JSON::Object();
		namelists->add(namelist);

		dictionary()->mapToJSON(**it, namelist);
	}

	return namelists;
}

}} //  Namespace MQ::Web
