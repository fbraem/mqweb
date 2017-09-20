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

	CommandServer* commandServer();
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

	Poco::JSON::Object& meta();
		/// Returns the JSON object for storing meta data (see JSON-API).
		/// This object can be used to store common data like queuemanager name,
		/// queue, elapsed time, ...

protected:


	void handle(const std::vector<std::string>& parameters, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);


private:


	QueueManagerPoolGuard::Ptr _qmgrPoolGuard;


	CommandServer* _commandServer;


	Poco::JSON::Object::Ptr _meta;


	Poco::Stopwatch _stopwatch;
};


inline QueueManager::Ptr MQController::qmgr()
{
	return _qmgrPoolGuard->getObject();
}


inline CommandServer* MQController::commandServer()
{
	return _commandServer;
}

inline Poco::JSON::Object& MQController::meta()
{
	poco_assert_dbg(_meta);

	return *_meta;
}

}} // Namespace MQ::Web

#endif //  _MQWeb_MQController_h
