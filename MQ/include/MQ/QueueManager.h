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
#ifndef _MQ_QueueManager_h
#define _MQ_QueueManager_h

#include <cmqc.h>
#include <cmqxc.h>

#include <string>

#include "Poco/SharedPtr.h"
#include "Poco/DateTime.h"

#include "Poco/Dynamic/Struct.h"

#include "MQ/CommandServer.h"

namespace MQ {

class QueueManager
	/// Represents a Websphere MQ Queuemanager
{
public:
	typedef Poco::SharedPtr<QueueManager> Ptr;

	QueueManager(const std::string& name = "");
		/// Constructor.

	virtual ~QueueManager();
		/// Destructor. Disconnects the queuemanager when it is still connected.

	void connect();
		/// Connects to the queuemanager. Can throw an MQException.

	void connect(const std::string& channel, const std::string& server);
		/// Connects to the queuemanager. Only use this method when the Websphere
		/// MQ system is loaded in client mode. Can throw an MQException.

	void connect(const std::string& channel, const std::string& server, const Poco::DynamicStruct& ssl);
		/// Connects to the queuemanager using a channel that is protected with SSL.
		/// The configuration for SSL is based on the following properties: keyrepos (required),
		/// cipherspec, fips, suiteb and certificate_validation_policy (only when
		/// MQSCO_VERSION_4 is defined).
		/// Only use this method when MQ system is loaded in client mode. Can
		/// throw an MQException.

	void connect(const Poco::DynamicStruct& connectionInformation);
		/// Connect using the information stored in the Poco::Dynamic::Struct object
		/// Can throw an MQException

	bool connected() const;
		/// Returns true when the queuemanager is connected.

	void disconnect();
		/// Disconnects from the queuemanager. Can throw an MQException.
		/// When a CommandServer object is owned by this queuemanager, it will
		/// be destroyed.

	std::string name() const;
		/// Returns the name of the queuemanager. The name is always inquired
		/// from the queuemanager when the connection is made.

	std::string id() const;
		/// Returns the queuemanager id. The id is always inquired from the
		/// queuemanager when the connection is made.

	std::string commandQueue() const;
		/// Returns the name of the command queue. The command queue is always
		/// required from the queuemanager when the connection is made.

	bool zos() const;
		/// Returns true when the queuemanager is running on Z/OS. This is
		/// detected by inquiring the platform property after the connection is made.

private:

	MQHCONN _handle;

	MQHCONN handle() const;

	std::string _name;

	std::string _id;

	std::string _commandQueue;

	MQLONG _applicationType;

	void inquireQmgrAttrs();

	friend class Queue;
	friend class MessageConsumer;
	friend class Topic;
};


inline std::string QueueManager::name() const
{
	return _name;
}


inline std::string QueueManager::id() const
{
	return _id;
}


inline std::string QueueManager::commandQueue() const
{
	return _commandQueue;
}

inline bool QueueManager::connected() const
{
	return _handle != 0L;
}

inline MQHCONN QueueManager::handle() const
{
	return _handle;
}

inline bool QueueManager::zos() const
{
	return _applicationType == MQPL_ZOS;
}

} // Namespace MQ

#endif // _MQ_QueueManager_h
