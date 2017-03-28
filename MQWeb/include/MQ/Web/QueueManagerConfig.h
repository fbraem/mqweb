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
#ifndef _MQ_QueueManagerConfig_h
#define _MQ_QueueManagerConfig_h

#include "Poco/Dynamic/Struct.h"

namespace MQ {
namespace Web {

class QueueManagerConfig
	/// Base class for queuemanager configuration.
{
public:

	QueueManagerConfig(const std::string& qmgrName);
		/// Constructor.

	virtual ~QueueManagerConfig();
		/// Destructor.

	std::string qmgrName() const;
		/// Returns the queuemanager name

	virtual Poco::DynamicStruct read() = 0;
		/// Read the configuration

private:

	std::string _qmgrName;
};

inline std::string QueueManagerConfig::qmgrName() const
{
	return _qmgrName;
}

}} // Namespace MQ::Web

#endif // _MQ_QueueManagerConfig_h
