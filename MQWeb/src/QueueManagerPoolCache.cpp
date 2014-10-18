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
#include "MQ/Web/QueueManagerPoolCache.h"

namespace MQ {
namespace Web {

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
{
}

QueueManagerFactory::~QueueManagerFactory()
{
}

QueueManager::Ptr QueueManagerFactory::createObject()
{
	return new QueueManager(_qmgrName);
}

void QueueManagerFactory::activateObject(QueueManager::Ptr qmgr)
{
	if ( !qmgr->connected() )
	{
		if ( _connectionInformation.size() == 0 )
		{
			qmgr->connect();
		}
		else
		{
			qmgr->connect(_connectionInformation);
		}
	}
}

QueueManagerPoolCache::QueueManagerPoolCache()
{
}

QueueManagerPoolCache::~QueueManagerPoolCache()
{
}

Poco::SharedPtr<QueueManagerPool> QueueManagerPoolCache::getPool(const std::string& qmgrName)
{
	return _cache.get(qmgrName);
}

Poco::SharedPtr<QueueManagerPool> QueueManagerPoolCache::createPool(const std::string& qmgrName, const Poco::Dynamic::Struct<std::string>& connectionInformation)
{
	QueueManagerFactory qmgrFactory(qmgrName, connectionInformation);
	Poco::SharedPtr<QueueManagerPool> pool = new QueueManagerPool(qmgrFactory, 10, 20);
	_cache.add(qmgrName, pool);
	return pool;
}

} } // Namespace MQ
