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

	CommandServer* commandServer();
		/// Returns the command server for this queuemanager. A command server
		/// must be created first with createCommandServer.

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

	CommandServer* createCommandServer(const std::string& replyQ);
		/// Create a command server. Once created, you can use commandServer to 
		/// get the associated command server. Can throw an MQException. The
		/// QueueManager instance is responsible for the CommandServer object
		/// and will destroy it when the queuemanager is disconnected.

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

	CommandServer* _commandServer;

	void inquireQmgrAttrs();

	friend class Queue;
	friend class MessageConsumer;
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

inline CommandServer* QueueManager::commandServer()
{
	return _commandServer;
}

inline CommandServer* QueueManager::createCommandServer(const std::string& replyQ)
{
	_commandServer = new CommandServer(*this, replyQ);
	return _commandServer;
}

} // Namespace MQ

#endif // _MQ_QueueManager_h
