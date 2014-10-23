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

namespace Poco {

template <class C>
class TimedObject
{
public:

	typedef SharedPtr<TimedObject<C> > Ptr;

	TimedObject(const C& object) : _object(object)
	{
	}

	~TimedObject()
	{
	}

	C& getObject()
	{
		return _object;
	}

	void updateLastUsed()
	{
		_lastUsed.update();
	}

private:

	C _object;

	Poco::Timestamp _lastUsed;
};


template <class C, class P, class F >
class ObjectPool<TimedObject<C>, P, F>
	/// An ObjectPool manages a pool of objects of a certain class.
	///
	/// The number of objects managed by the pool can be restricted.
	///
	/// When an object is requested from the pool:
	///   - If an object is available from the pool, an object from the pool is
	///     removed from the pool, activated (using the factory) and returned.
	///   - Otherwise, if the peak capacity of the pool has not yet been reached,
	///     a new object is created and activated, using the object factory, and returned.
	///   - If the peak capacity has already been reached, null is returned.
	///
	/// When an object is returned to the pool:
	///   - If the object is valid (checked by calling validateObject()
	///     from the object factory), the object is deactivated. If the
	///     number of objects in the pool is below the capacity,
	///     the object is added to the pool. Otherwise it is destroyed.
	///   - If the object is not valid, it is destroyed immediately.
{
public:

	ObjectPool(std::size_t capacity, std::size_t peakCapacity):
		/// Creates a new ObjectPool with the given capacity
		/// and peak capacity.
		///
		/// The PoolableObjectFactory must have a public default constructor.
		_capacity(capacity),
		_peakCapacity(peakCapacity),
		_size(0)
	{
		poco_assert (capacity <= peakCapacity);
	}

	ObjectPool(const F& factory, std::size_t capacity, std::size_t peakCapacity):
		/// Creates a new ObjectPool with the given PoolableObjectFactory,
		/// capacity and peak capacity. The PoolableObjectFactory must have
		/// a public copy constructor.
		_factory(factory),
		_capacity(capacity),
		_peakCapacity(peakCapacity),
		_size(0)
	{
		poco_assert (capacity <= peakCapacity);
	}

	~ObjectPool()
		/// Destroys the ObjectPool.
	{
		for (typename std::vector<P>::iterator it = _pool.begin(); it != _pool.end(); ++it)
		{
			_factory.destroyObject(*it);
		}
	}

	P borrowObject()
		/// Obtains an object from the pool, or creates a new object if
		/// possible.
		///
		/// Returns null if no object is available.
		///
		/// If activating the object fails, the object is destroyed and
		/// the exception is passed on to the caller.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		if (!_pool.empty())
		{
			P pObject = _pool.back();
			_pool.pop_back();
			return activateObject(pObject);
		}
		else if (_size < _peakCapacity)
		{
			P pObject = _factory.createObject();
			activateObject(pObject);
			_size++;
			return pObject;
		}
		else return 0;
	}

	void returnObject(P pObject)
		/// Returns an object to the pool.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		if (_factory.validateObject(pObject))
		{
			_factory.deactivateObject(pObject);
			if (_pool.size() < _capacity)
			{
				_pool.push_back(pObject);
			}
			else
			{
				_factory.destroyObject(pObject);
				_size--;
			}
		}
		else
		{
			_factory.destroyObject(pObject);
		}
	}

	std::size_t capacity() const
	{
		return _capacity;
	}

	std::size_t peakCapacity() const
	{
		return _peakCapacity;
	}

	std::size_t size() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		return _size;
	}

	std::size_t available() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		return _pool.size() + _peakCapacity - _size;
	}

protected:
	P activateObject(P pObject)
	{
		try
		{
			_factory.activateObject(pObject);
			pObject->updateLastUsed();
		}
		catch (...)
		{
			_factory.destroyObject(pObject);
			throw;
		}
		return pObject;
	}

private:
	ObjectPool();
	ObjectPool(const ObjectPool&);
	ObjectPool& operator = (const ObjectPool&);

	F _factory;
	std::size_t _capacity;
	std::size_t _peakCapacity;
	std::size_t _size;
	std::vector<P> _pool;
	mutable Poco::FastMutex _mutex;
};

} // namespace Poco


namespace MQ {

typedef Poco::TimedObject<QueueManager::Ptr> TimedQueueManager;

class QueueManagerFactory : public Poco::PoolableObjectFactory<TimedQueueManager, TimedQueueManager::Ptr>
{
public:
	QueueManagerFactory(const std::string& qmgrName);

	QueueManagerFactory(const std::string& qmgrName, const Poco::DynamicStruct& connectionInformation);

	QueueManagerFactory(const QueueManagerFactory& factory);

	virtual ~QueueManagerFactory();

	TimedQueueManager::Ptr createObject();

	void activateObject(TimedQueueManager::Ptr qmgr);

private:

	std::string _qmgrName;

	Poco::DynamicStruct _connectionInformation;
};


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

} // Namespace MQ

namespace MQ {
namespace Web {

typedef Poco::ObjectPool<TimedQueueManager, TimedQueueManager::Ptr, QueueManagerFactory> QueueManagerPool;
typedef PoolGuard<QueueManagerPool, TimedQueueManager::Ptr> QueueManagerPoolGuard;

}} // namespace MQ::Web

#endif // _MQ_QueueManagerPool_h
