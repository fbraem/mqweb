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

#include "Poco/ExpireCache.h"
#include "Poco/Dynamic/Struct.h"
#include "MQ/QueueManagerPool.h"

namespace MQ {
namespace Web {

class QueueManagerPoolCache
	/// This class implements a cache for queuemanager pools
	/// Create only one instance of this class.
	/// Make it a private member of the Application class for example
	/// and use the static instance() method to get this instance.
{
public:

	QueueManagerPoolCache();
		/// Constructor.

	virtual ~QueueManagerPoolCache();
		/// Destructor.

	void clear();
		/// Clear the cache

	QueueManagerPool::Ptr getQueueManagerPool(const std::string& qmgrName);
		/// Get a QueueManagerPool from the cache

	static QueueManagerPoolCache* instance();
		/// Returns the only instance of this class

private:

	QueueManagerPool::Ptr createPool(const std::string& qmgrName);

	void setup();

	Poco::ExpireCache<std::string, QueueManagerPool> _cache;

	Poco::Mutex _mutex;

	static QueueManagerPoolCache* _instance;
};

inline QueueManagerPoolCache* QueueManagerPoolCache::instance()
{
	return _instance;
}

}} // Namespace MQ::Web

#endif // _MQ_QueueManagerPoolCache_h
