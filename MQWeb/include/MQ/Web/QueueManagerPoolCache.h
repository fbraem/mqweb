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
#ifndef _MQ_QueueManagerPoolCache_h
#define _MQ_QueueManagerPoolCache_h

#include "Poco/AccessExpireCache.h"
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

	std::set<std::string> getAllPoolNames();

	static QueueManagerPoolCache* instance();
		/// Returns the only instance of this class

private:

	QueueManagerPool::Ptr createPool(const std::string& qmgrName);

	void setup();

	Poco::AccessExpireCache<std::string, QueueManagerPool> _cache;

	Poco::Mutex _mutex;

	static QueueManagerPoolCache* _instance;
};

inline std::set<std::string> QueueManagerPoolCache::getAllPoolNames()
{
	return _cache.getAllKeys();
}

inline QueueManagerPoolCache* QueueManagerPoolCache::instance()
{
	return _instance;
}

}} // Namespace MQ::Web

#endif // _MQ_QueueManagerPoolCache_h
