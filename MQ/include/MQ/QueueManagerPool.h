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

#include "MQ/QueueManager.h"

namespace MQ {

template <class C>
class TimedObject
	/// Helper class for storing the last used timestamp
{
public:

	TimedObject(Poco::SharedPtr<C> object) : _object(object)
	{
	}

	~TimedObject()
	{
	}

	Poco::SharedPtr<C> value()
	{
		return _object;
	}

	Poco::Timestamp lastused()
	{
		return _lastUsed;
	}

	void updateLastUsed()
	{
		_lastUsed.update();
	}

private:

	Poco::SharedPtr<C> _object;

	Poco::Timestamp _lastUsed;
};

typedef TimedObject<QueueManager::Ptr> TimedQueueManager;

class QueueManagerFactory
{
public:
	QueueManagerFactory(const std::string& qmgrName);

	QueueManagerFactory(const std::string& qmgrName, const Poco::DynamicStruct& connectionInformation);

	QueueManagerFactory(const QueueManagerFactory& factory);

	virtual ~QueueManagerFactory();

	QueueManager::Ptr createObject();

	void activateObject(QueueManager::Ptr qmgr);

	bool validateObject(QueueManager::Ptr pObject);

	void deactivateObject(QueueManager::Ptr pObject);

	void destroyObject(QueueManager::Ptr qmgr);

private:

	std::string _qmgrName;

	Poco::DynamicStruct _connectionInformation;
};

class QueueManagerPool
{
public:

	QueueManagerPool(Poco::SharedPtr<QueueManagerFactory> factory, std::size_t capacity, std::size_t peakCapacity);
		/// Constructor

	~QueueManagerPool();
		/// Destroys the ObjectPool.

	QueueManager::Ptr borrowObject();
		/// Obtains an object from the pool, or creates a new object if
		/// possible.
		///
		/// Returns null if no object is available.
		///
		/// If activating the object fails, the object is destroyed and
		/// the exception is passed on to the caller.

	void returnObject(QueueManager::Ptr pObject);
		/// Returns an object to the pool.

	std::size_t capacity() const;

	std::size_t peakCapacity() const;

	std::size_t size() const;

	std::size_t available() const;

protected:
	QueueManager::Ptr activateObject(QueueManager::Ptr pObject);

private:
	QueueManagerPool();
	QueueManagerPool(const QueueManagerPool&);
	QueueManagerPool& operator = (const QueueManagerPool&);

	Poco::SharedPtr<QueueManagerFactory> _factory;
	std::size_t _capacity;
	std::size_t _peakCapacity;
	std::size_t _size;
	std::vector<Poco::SharedPtr<TimedQueueManager> > _pool;
	mutable Poco::FastMutex _mutex;
};

inline std::size_t QueueManagerPool::capacity() const
{
	return _capacity;
}

inline std::size_t QueueManagerPool::peakCapacity() const
{
	return _peakCapacity;
}

inline std::size_t QueueManagerPool::size() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	return _size;
}

inline std::size_t QueueManagerPool::available() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	return _pool.size() + _peakCapacity - _size;
}


template<class Pool, class PObject>
class PoolGuard
{
public:

	typedef Poco::SharedPtr<PoolGuard<Pool, PObject> > Ptr;

	PoolGuard(Poco::SharedPtr<Pool> pool) : _pool(pool), _object(pool->borrowObject())
	{
	}

	~PoolGuard()
	{
		if ( ! _object.isNull() ) _pool->returnObject(_object);
	}

	PObject& getObject()
	{
		return _object;
	}

private:
	PoolGuard(const PoolGuard&);
	PoolGuard& operator= (const PoolGuard&);

	Poco::SharedPtr<Pool> _pool;

	PObject _object;
};

typedef PoolGuard<QueueManagerPool, QueueManager::Ptr> QueueManagerPoolGuard;

} // namespace MQ

#endif // _MQ_QueueManagerPool_h
