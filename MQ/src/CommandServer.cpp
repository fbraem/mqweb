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

#include <iostream>

#include "MQ/CommandServer.h"
#include "MQ/QueueManager.h"
#include "MQ/Queue.h"
#include "MQ/MQException.h"
#include "MQ/Message.h"
#include "MQ/PCF.h"

// When we get a MQRC_TRUNCATE..., we will try to enlarge the buffer.
// It is still possible to get MQRC_CONVERTED_MSG_TOO_BIG, but we really hope
// this buffer is large enough for all possible PCF answers.
#define REPLY_MESSAGE_LEN 8192

namespace MQ
{

CommandServer::CommandServer(QueueManager& qmgr, const std::string& modelQueue) 
	: _qmgr(qmgr)
	, _commandQ(qmgr, qmgr.commandQueue())
	, _replyQ(qmgr, modelQueue)
{
	_commandQ.open(MQOO_OUTPUT);
	_replyQ.open(MQOO_INPUT_SHARED | MQOO_FAIL_IF_QUIESCING);
}


PCF::Ptr CommandServer::createCommand(MQLONG command) const
{
	return new PCF(command, _qmgr.zos());
}

// Throws MQException
void CommandServer::sendCommand(PCF::Ptr& command, PCF::Vector& response)
{
	response.clear();

	command->setReplyToQueue(_replyQ.name());
	_commandQ.put(*command, MQPMO_NO_SYNCPOINT);

	long wait = 600000;
	PCF::Ptr msgResponse;

	bool keepRunning = true;
	while(keepRunning)
	{
		msgResponse = new PCF(_qmgr.zos());
		msgResponse->correlationId()->set(command->messageId());
		msgResponse->buffer().resize(REPLY_MESSAGE_LEN, false);

		try
		{
			_replyQ.get(*msgResponse.get(), MQGMO_CONVERT | MQGMO_NO_SYNCPOINT, wait);
		}
		catch(MQException& mqe)
		{
			if ( mqe.reason() == MQRC_TRUNCATED_MSG_FAILED )
			{
				Poco::Logger& logger = Poco::Logger::get("mq");
				if ( logger.trace() )
				{
					poco_trace_f2(logger, "Truncated message received. Actual size is %ld (> %d).", msgResponse->dataLength(), REPLY_MESSAGE_LEN);
				}
				
				msgResponse->buffer().resize(msgResponse->dataLength(), false);
				msgResponse->clear();
				msgResponse->correlationId()->set(command->messageId());
				_replyQ.get(*msgResponse.get(), MQGMO_CONVERT | MQGMO_NO_SYNCPOINT);
			}
			else
			{
				// on z/OS we can't rely on isLast, because a response
				// can have multiple sets of responses which have a
				// last flag in each set ...
				if ( _qmgr.zos() && mqe.reason() == MQRC_NO_MSG_AVAILABLE )
				{
					keepRunning = false;
					continue;
				}

				throw;
			}
		}
		wait = 100;
		msgResponse->buffer().resize(msgResponse->dataLength());
		msgResponse->init();

		response.push_back(msgResponse);

		if ( ! _qmgr.zos() && msgResponse->isLast() )
		{
			keepRunning = false;
		}
	}
}

}
