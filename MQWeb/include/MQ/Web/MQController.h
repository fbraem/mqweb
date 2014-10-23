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

#ifndef _MQWeb_MQController_h
#define _MQWeb_MQController_h

#include "Poco/Stopwatch.h"

#include "MQ/QueueManager.h"
#include "MQ/CommandServer.h"
#include "MQ/MQException.h"
#include "MQ/Web/Controller.h"
#include "MQ/Web/QueueManagerPoolCache.h"

namespace MQ {
namespace Web {

class MQController : public Controller
	/// Controller for Websphere MQ
{
public:

	MQController();
		/// Constructor

	virtual ~MQController();
		/// Destructor

	QueueManager::Ptr qmgr();
		/// Returns the associated queuemanager

	CommandServer::Ptr commandServer();
		/// Returns the associated command server

	void handleException(const MQException& mqe);
		/// Handles an MQException. Renders the error template or
		/// returns an error as JSON object.

	void beforeAction();
		/// Connects to the queuemanager and command server

	void afterAction();
		/// Stops the stopwatch and calls Controller::afterAction

	void handleFilterForm(Poco::JSON::Object::Ptr pcfParameters);
		/// Creates an IntegerFilterCommand or StringFilterCommand
		/// when a filter is passed.

	Poco::JSON::Object& mqwebData();
		/// Returns the JSON object for storing MQWeb data.
		/// This object can be used to store common data like queuemanager name,
		/// queue, elapsed time, ...

protected:


	void handle(const std::vector<std::string>& parameters, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);


private:


	QueueManagerPoolGuard::Ptr _qmgrPoolGuard;


	CommandServer::Ptr _commandServer;


	Poco::JSON::Object::Ptr _mqwebData;


	Poco::Stopwatch _stopwatch;


	static QueueManagerPoolCache _cache;
};


inline QueueManager::Ptr MQController::qmgr()
{
	return _qmgrPoolGuard->getObject()->getObject();
}


inline CommandServer::Ptr MQController::commandServer()
{
	return _commandServer;
}

inline Poco::JSON::Object& MQController::mqwebData()
{
	poco_assert_dbg(_mqwebData);

	return *_mqwebData;
}

}} // Namespace MQ::Web

#endif //  _MQWeb_MQController_h
