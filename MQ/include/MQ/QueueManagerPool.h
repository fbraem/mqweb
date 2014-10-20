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


class QueueManagerFactory : public Poco::PoolableObjectFactory<QueueManager, QueueManager::Ptr>
{
public:
	QueueManagerFactory(const std::string& qmgrName);

	QueueManagerFactory(const std::string& qmgrName, const Poco::DynamicStruct& connectionInformation);

	QueueManagerFactory(const QueueManagerFactory& factory);

	virtual ~QueueManagerFactory();

	QueueManager::Ptr createObject();

	void activateObject(QueueManager::Ptr qmgr);

private:

	std::string _qmgrName;

	Poco::DynamicStruct _connectionInformation;
};



class QueueManagerPool
	/// This class implements a Queuemanager pool
{
public:
	QueueManagerPool(const QueueManagerFactory& factory, std::size_t capacity, std::size_t peakCapacity);
		/// Constructor.

	virtual ~QueueManagerPool();
		/// Destructor.

	QueueManager::Ptr getQueueManager();

	void release(QueueManager::Ptr qmgr);

private:

	Poco::ObjectPool<QueueManager, QueueManager::Ptr, QueueManagerFactory> _pool;
};

inline QueueManager::Ptr QueueManagerPool::getQueueManager()
{
	return _pool.borrowObject();
}

inline void QueueManagerPool::release(QueueManager::Ptr qmgr)
{
	_pool.returnObject(qmgr);
}


class QueueManagerPoolGuard
{
public:
	QueueManagerPoolGuard(Poco::SharedPtr<QueueManagerPool> pool);

	~QueueManagerPoolGuard();

	QueueManager::Ptr getQueueManager() const;

private:
	QueueManagerPoolGuard(const QueueManagerPoolGuard&);
	QueueManagerPoolGuard& operator= (const QueueManagerPoolGuard&);

	Poco::SharedPtr<QueueManagerPool> _pool;

	QueueManager::Ptr _qmgr;
};

inline QueueManager::Ptr QueueManagerPoolGuard::getQueueManager() const
{
	return _qmgr;
}

} // Namespace MQ

#endif // _MQ_QueueManagerPool_h
