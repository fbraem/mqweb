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

#include "MQ/QueueManagerPool.h"

namespace MQ
{

QueueManagerFactory::QueueManagerFactory(const std::string& qmgrName)
: _qmgrName(qmgrName)
{
}

QueueManagerFactory::QueueManagerFactory(const std::string& qmgrName, const Poco::Dynamic::Struct<std::string>& connectionInformation)
: _qmgrName(qmgrName)
, _connectionInformation(connectionInformation)
{
}

QueueManagerFactory::QueueManagerFactory(const QueueManagerFactory& copy)
: _qmgrName(copy._qmgrName)
, _connectionInformation(copy._connectionInformation)
{
}

QueueManagerFactory::~QueueManagerFactory()
{
}

TimedQueueManager::Ptr QueueManagerFactory::createObject()
{
	return new TimedQueueManager(new QueueManager(_qmgrName));
}

void QueueManagerFactory::activateObject(TimedQueueManager::Ptr qmgr)
{
	if ( !qmgr->getObject()->connected() )
	{
		if ( _connectionInformation.size() == 0 )
		{
			qmgr->getObject()->connect();
		}
		else
		{
			qmgr->getObject()->connect(_connectionInformation);
		}
	}
}

} // Namespace MQ
