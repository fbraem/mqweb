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

bool QueueManagerFactory::validateObject(QueueManager::Ptr pObject)
{
	return pObject->connected();
}

void QueueManagerFactory::deactivateObject(QueueManager::Ptr pObject)
{
}

void QueueManagerFactory::destroyObject(QueueManager::Ptr pObject)
{
	try
	{
		pObject->disconnect();
	}
	catch(...)
	{
	}
}

QueueManagerPool::QueueManagerPool(Poco::SharedPtr<QueueManagerFactory> factory,
	std::size_t capacity,
	std::size_t peakCapacity,
	int idleTime) :
	_factory(factory),
	_capacity(capacity),
	_peakCapacity(peakCapacity),
	_size(0),
	_idleTime(idleTime),
	_janitorTimer(1000 * idleTime, 1000 * idleTime / 4)
{
	poco_assert(capacity <= peakCapacity);
	Poco::TimerCallback<QueueManagerPool> callback(*this, &QueueManagerPool::onJanitorTimer);
	_janitorTimer.start(callback);
}

QueueManagerPool::~QueueManagerPool()
{
	for (std::vector<Poco::SharedPtr<TimedQueueManager> >::iterator it = _pool.begin(); it != _pool.end(); ++it)
	{
		_factory->destroyObject((*it)->value());
	}
}

QueueManager::Ptr QueueManagerPool::borrowObject()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (!_pool.empty())
	{
		Poco::SharedPtr<TimedQueueManager> pObject = _pool.back();
		_pool.pop_back();
		return activateObject(pObject->value());
	}

	if (_size < _peakCapacity)
	{
		QueueManager::Ptr pObject = _factory->createObject();
		activateObject(pObject);
		_size++;
		return pObject;
	}

	return 0;
}

void QueueManagerPool::returnObject(QueueManager::Ptr pObject)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (_factory->validateObject(pObject))
	{
		_factory->deactivateObject(pObject);
		if (_pool.size() < _capacity)
		{
			Poco::SharedPtr<TimedQueueManager> timed = new TimedQueueManager(pObject);
			_pool.push_back(timed);
		}
		else
		{
			_factory->destroyObject(pObject);
			_size--;
		}
	}
	else
	{
		_factory->destroyObject(pObject);
	}
}

QueueManager::Ptr QueueManagerPool::activateObject(QueueManager::Ptr pObject)
{
	try
	{
		_factory->activateObject(pObject);
	}
	catch (...)
	{
		_factory->destroyObject(pObject);
		throw;
	}
	return pObject;
}

void QueueManagerPool::onJanitorTimer(Poco::Timer&)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	std::vector<Poco::SharedPtr<TimedQueueManager> >::iterator it = _pool.begin();
	while(it != _pool.end())
	{
		if ( (*it)->idle() > _idleTime )
		{
			_factory->destroyObject((*it)->value());
			_size--;
			it = _pool.erase(it);
		}
		else ++it;
	}
}

} // Namespace MQ
