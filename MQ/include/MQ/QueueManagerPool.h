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
#ifndef _MQ_QueueManagerPool_h
#define _MQ_QueueManagerPool_h

#include "Poco/ObjectPool.h"

#include "MQ/QueueManager.h"


namespace MQ {


class QueueManagerPool
	/// This class implements a Queuemanager pool
{
public:
	typedef Poco::PoolableObjectFactory<QueueManager, QueueManager::Ptr> QueueManagerFactory;

	QueueManagerPool(const QueueManagerFactory& factory, std::size_t capacity, std::size_t peakCapacity);
		/// Constructor.

	virtual ~QueueManagerPool();
		/// Destructor.

private:

	Poco::ObjectPool<QueueManager, QueueManager::Ptr, QueueManagerFactory> _pool;
};

} // Namespace MQ

#endif // _MQ_QueueManagerPool_h
