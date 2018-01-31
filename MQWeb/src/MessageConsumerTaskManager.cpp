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
#include "Poco/Logger.h"

#include "MQ/Web/QueueManagerPoolCache.h"

#include "MQ/Web/MessageConsumerTaskManager.h"
#include "MQ/Web/MessageConsumerTask.h"

namespace MQ {
namespace Web {

MessageConsumerTaskManager::MessageConsumerTaskManager(Poco::ThreadPool& threadPool) : _taskManager(threadPool)
{
}

MessageConsumerTaskManager::~MessageConsumerTaskManager()
{
	Poco::Logger& logger = Poco::Logger::get("mq.web");
	logger.trace("Cancel all active MessageConsumerTasks");

	try 
	{
		_taskManager.cancelAll();
		_taskManager.joinAll();
	}
	catch (Poco::Exception&)
	{

	}
}

void MessageConsumerTaskManager::startTask(Poco::SharedPtr<Poco::Net::WebSocket> ws, const std::string& qmgrName, const std::string& queueName)
{
	Poco::SharedPtr<QueueManagerPool> qmgrPool = QueueManagerPoolCache::instance()->getQueueManagerPool(qmgrName);
	if (qmgrPool.isNull())
	{
		throw Poco::OutOfMemoryException("Can't create a pool for queuemanager.");
	}

	QueueManager::Ptr qmgr = qmgrPool->borrowObject();
	if (qmgr.isNull())
	{
		throw Poco::OutOfMemoryException("No queuemanager available in the pool. Check the connection pool configuration.");
	}

	_taskManager.start(new MessageConsumerTask(ws, new QueueManagerPoolGuard(qmgrPool, qmgr), queueName));
}

}}