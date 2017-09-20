/*
* Copyright 2017 - KBC Group NV - Franky Braem - The MIT license
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef _MQ_QueueManagerPool_h
#define _MQ_QueueManagerPool_h

#include "Poco/Timer.h"

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

	int idle() const
	{
		return (int) (_lastUsed.elapsed() / Poco::Timestamp::resolution());
	}

	Poco::Timestamp lastused() const
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

typedef TimedObject<QueueManager> TimedQueueManager;

class QueueManagerFactory
	/// Factory class for creating a queuemanager
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

	typedef Poco::SharedPtr<QueueManagerPool> Ptr;

	QueueManagerPool(Poco::SharedPtr<QueueManagerFactory> factory,
		std::size_t capacity,
		std::size_t peakCapacity,
		int idleTime = 60);
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
	void onJanitorTimer(Poco::Timer&);

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

	int _idleTime;
	Poco::Timer _janitorTimer;
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

	PoolGuard(Poco::SharedPtr<Pool> pool, PObject object) : _pool(pool), _object(object)
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
