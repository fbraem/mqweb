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
#include "Poco/Util/Application.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Logger.h"

#include "MQ/MQSubsystem.h"

#include "MQ/Web/QueueManagerPoolCache.h"
#include "MQ/Web/QueueManagerDefaultConfig.h"
#include "MQ/Web/QueueManagerDatabaseConfig.h"

namespace MQ {
namespace Web {

QueueManagerPoolCache* QueueManagerPoolCache::_instance = NULL;

QueueManagerPoolCache::QueueManagerPoolCache()
{
	setup();
}

QueueManagerPoolCache::~QueueManagerPoolCache()
{
	_instance = NULL;
}

void QueueManagerPoolCache::setup()
{
	poco_assert(_instance == NULL);
	_instance = this;
}

QueueManagerPool::Ptr QueueManagerPoolCache::getQueueManagerPool(const std::string& qmgrName)
{
	QueueManagerPool::Ptr pool = _cache.get(qmgrName);
	if ( pool.isNull() )
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		pool = _cache.get(qmgrName); // Check it again ...
		if ( pool.isNull() )
		{
			pool = createPool(qmgrName);
		}
	}

	return pool;
}

void QueueManagerPoolCache::clear()
{
	_cache.clear();
}

QueueManagerPool::Ptr QueueManagerPoolCache::createPool(const std::string& qmgrName)
{
	QueueManagerPool::Ptr pool;

	MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();

	Poco::SharedPtr<QueueManagerFactory> factory;

	if ( mqSystem.client() )
	{
		QueueManagerDefaultConfig qmgrConfig(config);

		Poco::Logger& logger = Poco::Logger::get("mq.web");

		try
		{
			Poco::DynamicStruct connectionInformation = qmgrConfig.read(qmgrName);
			poco_information_f2(logger, "%s - Connection information: %s", qmgrName, connectionInformation.toString());
			factory = new QueueManagerFactory(qmgrName, connectionInformation);
		}
		catch(Poco::Exception& e)
		{
			logger.log(e);
		}
	}
	else
	{
		factory = new QueueManagerFactory(qmgrName);
	}

	if (!factory.isNull())
	{
		std::size_t capacity;
		std::size_t peakCapacity;
		int idle;

		std::string qmgrConfig = "mq.web.qmgr." + qmgrName;
		std::string qmgrPoolCapacity = qmgrConfig + ".pool.capacity";
		if ( !config.has(qmgrPoolCapacity) )
		{
			qmgrPoolCapacity = "mq.web.pool.capacity";
		}
		capacity = config.getInt(qmgrPoolCapacity, 10);

		std::string qmgrPoolPeakCapacity = qmgrConfig + ".pool.peakcapacity";
		if ( !config.has(qmgrPoolPeakCapacity) )
		{
			qmgrPoolPeakCapacity = "mq.web.pool.peakcapacity";
		}
		peakCapacity = config.getInt(qmgrPoolPeakCapacity, 20);

		std::string qmgrPoolIdle = qmgrConfig + ".pool.idle";
		if ( !config.has(qmgrPoolIdle) )
		{
			qmgrPoolIdle = "mq.web.pool.idle";
		}
		idle = config.getInt(qmgrPoolIdle, 60);

		pool = new QueueManagerPool(factory, capacity, peakCapacity, idle);

		_cache.add(qmgrName, pool);
	}

	return pool;
}

} } // Namespace MQ
