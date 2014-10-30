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
	return true;
}

void QueueManagerFactory::deactivateObject(QueueManager::Ptr pObject)
{
}

void QueueManagerFactory::destroyObject(QueueManager::Ptr pObject)
{
}

QueueManagerPool::QueueManagerPool(Poco::SharedPtr<QueueManagerFactory> factory, std::size_t capacity, std::size_t peakCapacity) :
	_factory(factory),
	_capacity(capacity),
	_peakCapacity(peakCapacity),
	_size(0)
{
	poco_assert(capacity <= peakCapacity);
}

QueueManagerPool::~QueueManagerPool()
{
	for (typename std::vector<Poco::SharedPtr<TimedQueueManager> >::iterator it = _pool.begin(); it != _pool.end(); ++it)
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

} // Namespace MQ
