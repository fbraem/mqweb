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
#ifndef _MQ_MQWebSubsystem_h
#define _MQ_MQWebSubsystem_h

#include "Poco/Util/Subsystem.h"

#include "MQ/Web/QueueManagerPoolCache.h"
#include "MQ/Web/DictionaryCache.h"
#include "MQ/Web/MessageConsumerTaskManager.h"

namespace MQ {
namespace Web {

class MQWebSubsystem : public Poco::Util::Subsystem
	/// A POCO Subsystem for MQWeb
{
public:
	MQWebSubsystem();
	/// Constructor

	virtual ~MQWebSubsystem();
	/// Destructor

	virtual const char* name() const;
	/// Returns the name

	virtual void initialize(Poco::Util::Application& app);
	/// Initialize the subsystem

	virtual void uninitialize();
	/// Unitialize the subsystem

	Poco::SharedPtr<QueueManagerPool> getQueueManagerPool(const std::string& qmgr);

	MQ::Web::MessageConsumerTaskManager& messageConsumerTaskManager();

private:

	QueueManagerPoolCache* _qmgrPoolCache;

	DictionaryCache* _dictionaryCache;

	Poco::ThreadPool* _messageConsumerThreadPool;

	MessageConsumerTaskManager* _messageConsumerTaskManager;
};


inline const char * MQWebSubsystem::name() const
{
	return "MQWeb";
}

inline MQ::Web::MessageConsumerTaskManager& MQWebSubsystem::messageConsumerTaskManager()
{
	return *_messageConsumerTaskManager;
}

}} // Namespace MQ::Web

#endif // _MQ_MQWebSubsystem_h
