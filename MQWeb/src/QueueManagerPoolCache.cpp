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
#include "Poco/Util/Application.h"

#include "MQ/MQSubsystem.h"

#include "MQ/Web/QueueManagerPoolCache.h"

namespace MQ {
namespace Web {


QueueManagerPoolCache::QueueManagerPoolCache()
{
}

QueueManagerPoolCache::~QueueManagerPoolCache()
{
}

Poco::SharedPtr<QueueManagerPool> QueueManagerPoolCache::getQueueManagerPool(const std::string& qmgrName)
{
	Poco::SharedPtr<QueueManagerPool> pool = _cache.get(qmgrName);
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

Poco::SharedPtr<QueueManagerPool> QueueManagerPoolCache::createPool(const std::string& qmgrName)
{
	Poco::SharedPtr<QueueManagerPool> pool;

	MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();

	std::string qmgrConfig = "mq.web.qmgr." + qmgrName;

	if ( mqSystem.client() )
	{
		Poco::DynamicStruct connectionInformation;

		// In client mode we check for a configuration
		// When this is not available, we hope that a channel tab file
		// is configured.
		std::string qmgrConfigConnection = qmgrConfig + ".connection";
		if ( config.has(qmgrConfigConnection) )
		{
			std::string connection;
			std::string channel;
			connectionInformation.insert("connection", config.getString(qmgrConfigConnection));
			std::string qmgrConfigChannel = qmgrConfig + ".channel";
			if ( config.has(qmgrConfigChannel) )
			{
				connectionInformation.insert("channel", config.getString(qmgrConfigChannel));
			}
			else
			{
				connectionInformation.insert("channel", config.getString("mq.web.defaultChannel", "SYSTEM.DEF.SVRCONN"));
			}

			if ( config.has("mq.web.ssl.keyrepos") )
			{
				Poco::DynamicStruct ssl;
				connectionInformation.insert("ssl", ssl);

				Poco::Util::AbstractConfiguration::Keys keys;
				Poco::AutoPtr<Poco::Util::AbstractConfiguration> sslConfig = config.createView("mq.web.ssl");
				sslConfig->keys(keys);
				for(Poco::Util::AbstractConfiguration::Keys::iterator it = keys.begin(); it != keys.end(); ++it)
				{
					ssl.insert(*it, config.getString(*it));
				}
			}

			QueueManagerFactory qmgrFactory(qmgrName, connectionInformation);
			pool = new QueueManagerPool(qmgrFactory, 10, 20);
		}
		else
		{
			QueueManagerFactory qmgrFactory(qmgrName);
			pool = new QueueManagerPool(qmgrFactory, 10, 20);
		}
	}
	else
	{
		QueueManagerFactory qmgrFactory(qmgrName);
		pool = new QueueManagerPool(qmgrFactory, 10, 20);
	}

	_cache.add(qmgrName, pool);
	return pool;
}

} } // Namespace MQ
