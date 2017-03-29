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
#ifndef _MQ_QueueManagerDatabaseConfig_h
#define _MQ_QueueManagerDatabaseConfig_h

#include "MQ/Web/QueueManagerConfig.h"

#include "Poco/Data/Session.h"

namespace MQ {
namespace Web {

class QueueManagerDatabaseConfig : public QueueManagerConfig
	/// Class that retrieves queuemanager configuration from a database.
{
public:

	QueueManagerDatabaseConfig(const std::string& qmgrName, const std::string& connector, const std::string& connectionString);
		/// Constructor.

	virtual ~QueueManagerDatabaseConfig();
		/// Destructor.

	void list(std::vector<std::string>& arr) const;
		/// Returns all configured queuemanagers

	Poco::DynamicStruct read();
		/// Read the configuration

private:

	mutable Poco::SharedPtr<Poco::Data::Session> _session;
};

}} // Namespace MQ::Web

#endif // _MQ_QueueManagerDatabaseConfig_h
