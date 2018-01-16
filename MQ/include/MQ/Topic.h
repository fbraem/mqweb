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
#ifndef _MQ_Topic_H
#define _MQ_Topic_H

#include <cmqc.h>

#include "Poco/SharedPtr.h"
#include "Poco/String.h"

#include <string>
#include <vector>
#include <map>

#include "MQ/QueueManager.h"

namespace MQ
{
class Message;

class Topic
	/// Represents a Websphere MQ Topic to use for publish/subscribe
{
public:
	Topic(QueueManager::Ptr qmgr, const std::string& topic);
		/// Constructor. Use this for a fixed topic.

	Topic(QueueManager::Ptr qmgr, const std::string& topic, const std::string& topicStr);
		/// Constructor. Use this for a programmatically defined topic.

	virtual ~Topic();
		/// Destructor. Closes the topic when it is still open.

	void inquire(const std::vector<int>& intSelectors, const std::map<int, int>& charSelectors, std::map<int, int>& intResult, std::map<int, std::string>& charResult);
		/// Inquire

	std::string name() const;
		/// Returns the name of the topic.

	void open(long options);
		/// Open the topic with the given options. Can throw an MQException.

	void close();
		/// Close the topic. Can throw an MQException.

	void publish(Message& msg, MQLONG options = MQPMO_NONE);
		/// Publish a message to the topic. Can throw an MQException.

	std::string topicString() const;
		/// Returns the topicstring

	std::string resultTopicString() const;
		/// Returns the resolved topicstring

private:

	QueueManager::Ptr _qmgr;

	MQHOBJ _handle;

	MQHOBJ handle() const;

	MQOD _od;

	std::string _topic;

	std::string _topicStr;

	std::string _resObjectStr;

	static MQOD _initialOD;

	friend class QueueManager;
};

inline MQHOBJ Topic::handle() const
{
	return _handle;
}

inline std::string Topic::name() const
{
	return _topic;
}

inline std::string Topic::resultTopicString() const
{
	return _resObjectStr;
}

} // Namespace MQ

#endif // _MQ_Topic_H
