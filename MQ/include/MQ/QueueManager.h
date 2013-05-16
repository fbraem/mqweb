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

#include <Poco/SharedPtr.h>
#include <Poco/DateTime.h>

namespace MQ {

class QueueManager
{
public:
	typedef Poco::SharedPtr<QueueManager> Ptr;


	QueueManager(const std::string& name = "");


	virtual ~QueueManager();


	void connect();


	void connect(const std::string& channel, const std::string& server);


	void disconnect();


	std::string name() const;


	std::string id() const;


	std::string commandQueue() const;


	bool zos() const;


private:

	MQHCONN _handle;


	std::string _name;


	std::string _id;


	std::string _commandQueue;


	bool _zos;


	void inquireQmgrAttrs();


	friend class Queue;
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

inline bool QueueManager::zos() const
{
	return _zos;
}

} // Namespace MQ

#endif // _MQ_QueueManager_h
