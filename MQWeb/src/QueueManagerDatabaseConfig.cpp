/*
 * Copyright 2017 MQWeb - Franky Braem
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

#include "MQ/Web/QueueManagerDatabaseConfig.h"

namespace MQ {
namespace Web {

using namespace Poco::Data::Keywords;

QueueManagerDatabaseConfig::QueueManagerDatabaseConfig(const std::string& qmgrName, const std::string& connector, const std::string& connectionString)
: QueueManagerConfig(qmgrName), _session(new Poco::Data::Session(connector, connectionString))
{
}

QueueManagerDatabaseConfig::~QueueManagerDatabaseConfig()
{
}

void QueueManagerDatabaseConfig::list(std::vector<std::string>& arr) const
{
	typedef Poco::Tuple<std::string> Attribute;
	std::vector<Attribute> attributes;

	*_session << "SELECT name FROM queuemanagers", into(attributes), now;
	for(std::vector<Attribute>::iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		arr.push_back(it->get<0>());
	}
}

Poco::DynamicStruct QueueManagerDatabaseConfig::read()
{
	Poco::DynamicStruct connectionInformation;

	std::string name = qmgrName();
	std::string channel;
	std::string server;
	int port = 0;

	*_session << "SELECT channel, server, port FROM queuemanagers WHERE name = :n", into(channel), into(server), into(port), useRef(name), now;

	connectionInformation.insert("channel", channel);
	connectionInformation.insert("connection", Poco::format("%s(%d)", server, port));

	return connectionInformation;
}

} } // Namespace MQ::Web
