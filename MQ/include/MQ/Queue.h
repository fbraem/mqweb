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
	typedef Poco::SharedPtr<Queue> Ptr;

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

	void inquire(const std::vector<int>& intSelectors, const std::map<int, int>& charSelectors, std::map<int, int>& intResult, std::map<int, std::string>& charResult);
		/// Inquire

private:

	Poco::SharedPtr<QueueManager> _qmgr;

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
