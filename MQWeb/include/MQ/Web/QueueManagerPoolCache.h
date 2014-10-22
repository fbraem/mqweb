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
#ifndef _MQ_QueueManagerPoolCache_h
#define _MQ_QueueManagerPoolCache_h

#include "Poco/Dynamic/Struct.h"
#include "Poco/ExpireCache.h"
#include "MQ/QueueManagerPool.h"


namespace MQ {
namespace Web {

typedef Poco::ObjectPool<QueueManager, QueueManager::Ptr, QueueManagerFactory> QueueManagerPool;
typedef PoolGuard<QueueManagerPool, QueueManager> QueueManagerPoolGuard;

class QueueManagerPoolCache
	/// This class implements a cache for queuemanager pools
{
public:

	QueueManagerPoolCache();
		/// Constructor.

	virtual ~QueueManagerPoolCache();
		/// Destructor.

	Poco::SharedPtr<QueueManagerPoolGuard> getQueueManager(const std::string& qmgrName);

private:

	Poco::SharedPtr<QueueManagerPool> createPool(const std::string& qmgrName);

	Poco::ExpireCache<std::string, QueueManagerPool> _cache;

	Poco::Mutex _mutex;
};

}} // Namespace MQ::Web

#endif // _MQ_QueueManagerPoolCache_h
