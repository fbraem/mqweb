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
#include "MQ/Web/ProcessMapper.h"
#include "MQ/MQException.h"

#include "Poco/JSON/Object.h"

namespace MQ {
namespace Web {


ProcessMapper::ProcessMapper(CommandServer& commandServer) : MQMapper(commandServer, "Process")
{
}

ProcessMapper::~ProcessMapper()
{
}


void ProcessMapper::change(const Poco::JSON::Object::Ptr&obj)
{
	poco_assert_dbg(false); // Not yet implemented
}


void ProcessMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ProcessMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ProcessMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
	poco_assert_dbg(!filter.isNull());

	Command command(this, MQCMD_INQUIRE_PROCESS, filter);

	// Required parameters
	command.addParameter<std::string>(MQCA_PROCESS_NAME, "ProcessName");

	// Optional parameters
	command.addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	command.addIntegerFilter();
	command.addAttributeList(MQIACF_PROCESS_ATTRS, "ProcessAttrs");
	command.addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	command.addStringFilter();

	PCF::Vector commandResponse;
	command.execute(commandResponse);

	bool excludeSystem = filter->optValue("ExcludeSystem", false);

	Poco::JSON::Array::Ptr processes = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		std::string processName = (*it)->getParameterString(MQCA_PROCESS_NAME);
		if (   excludeSystem
			&& processName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		Poco::JSON::Object::Ptr process = new Poco::JSON::Object();
		processes->add(process);

		dictionary()->mapToJSON(**it, process);
	}

	return processes;
}

}} //  Namespace MQ::Web
