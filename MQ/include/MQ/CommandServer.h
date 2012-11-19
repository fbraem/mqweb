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

#ifndef _MQ_CommandServer_h
#define _MQ_CommandServer_h

#include <cmqc.h>
#include <vector>

#include <MQ/QueueManager.h>
#include <MQ/Queue.h>
#include <MQ/PCF.h>

#include <Poco/SharedPtr.h>

namespace MQ {

class CommandServer
{
public:
	CommandServer(QueueManager::Ptr qmgr, const std::string& modelQueue);


	PCF::Ptr createCommand(MQLONG command) const;


	void sendCommand(PCF::Ptr& command, PCF::Vector& response);


	const QueueManager& qmgr() const;


	typedef Poco::SharedPtr<CommandServer> Ptr;


private:
	QueueManager::Ptr _qmgr;


	Queue _commandQ;


	Queue _replyQ;
	
};


inline const QueueManager& CommandServer::qmgr() const
{
	poco_assert_dbg(!_qmgr.isNull()); // Can't be null
	return *_qmgr.get();
}

} // namespace MQ

#endif //  _MQ_CommandServer_h
