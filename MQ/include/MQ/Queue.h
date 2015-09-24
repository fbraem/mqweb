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

#ifndef _MQ_Queue_H
#define _MQ_Queue_H

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

class Queue
	/// Represents a Websphere MQ queue
{
public:
	Queue(QueueManager& qmgr, const std::string& name);
		/// Constructor

	virtual ~Queue();
		/// Destructor. Closes the queue when it is still open.

	std::string name() const;
		/// Returns the name of the queue

	void open(long options);
		/// Open the queue with the given options. Can throw an MQException.

	void close();
		/// Close the queue. Can throw an MQException.

	void put(Message& msg, MQLONG options = MQPMO_NONE);
		/// Puts the given message on the queue. Can throw an MQException.

	void get(Message& msg, MQLONG options = MQGMO_NONE, long wait = 0);
		/// Gets a message from the queue. Can throw an MQException.

	void inquire(const std::vector<int>& intSelectors, const std::map<int, int>& charSelectors, std::map<int, int>& intResult, std::map<int, std::string>& charResult);
		/// Inquire

private:

	QueueManager& _qmgr;

	MQHOBJ _handle;

	MQHOBJ handle() const;

	MQOD _od;

	static MQOD _initialOD;

	friend class QueueManager;
	friend class MessageConsumer;
};

inline MQHOBJ Queue::handle() const
{
	return _handle;
}

inline std::string Queue::name() const
{
	std::string s(_od.ObjectName, MQ_Q_NAME_LENGTH);

	size_t zero = s.find_first_of('\0');
	if ( zero != std::string::npos ) // MQ Strings can have only 0's
	{
		s = s.substr(0, zero);
	}

	Poco::trimRightInPlace(s);
	return s;
}

}
#endif // _MQ_Queue_H
