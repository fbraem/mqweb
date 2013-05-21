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

#include <Poco/SharedPtr.h>

#include <string>


namespace MQ
{
class QueueManager;
class Message;

class Queue
	/// Represents a Websphere MQ queue
{
public:
	Queue(Poco::SharedPtr<QueueManager> qmgr, const std::string& name);
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

private:

	Poco::SharedPtr<QueueManager> _qmgr;

	MQHOBJ _handle;

	MQOD _od;

	static MQOD _initialOD;

	friend class QueueManager;
};

inline std::string Queue::name() const
{
	std::string s(_od.ObjectName, MQ_Q_NAME_LENGTH - 1);
	s.erase(s.find_last_not_of(" \n\r\t")+1);
	return s;
}

}
#endif // _MQ_Queue_H
