/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they
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

#ifndef _MQ_Topic_H
#define _MQ_Topic_H

#include <cmqc.h>

#include "Poco/SharedPtr.h"
#include "Poco/String.h"

#include <string>
#include <vector>
#include <map>

namespace MQ
{
class QueueManager;
class Message;

class Topic
	/// Represents a Websphere MQ Topic to use for publish/subscribe
{
public:
	Topic(QueueManager& qmgr, const std::string& topic);
		/// Constructor. Use this for a fixed topic.

	Topic(QueueManager& qmgr, const std::string& topic, const std::string& topicStr);
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

	QueueManager& _qmgr;

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
