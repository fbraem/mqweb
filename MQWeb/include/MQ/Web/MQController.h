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

#include "MQ/QueueManager.h"
#include "MQ/CommandServer.h"
#include "MQ/MQException.h"
#include "MQ/Web/Controller.h"

namespace MQ {
namespace Web {

class MQController : public Controller
{
public:

	MQController();


	virtual ~MQController();


	QueueManager::Ptr qmgr();


	CommandServer::Ptr commandServer();


	void handleException(const MQException& mqe);


	void beforeAction();


private:


	QueueManager::Ptr _qmgr;


	CommandServer::Ptr _commandServer;


	bool connect();


	bool connect(const std::string& channel, const std::string& connection);

};


inline QueueManager::Ptr MQController::qmgr()
{
	return _qmgr;
}


inline CommandServer::Ptr MQController::commandServer()
{
	return _commandServer;
}

}} // Namespace MQ::Web

#endif //  _MQWeb_MQController_h
