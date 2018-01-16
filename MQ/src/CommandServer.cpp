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

CommandServer::CommandServer(Poco::SharedPtr<QueueManager> qmgr, const std::string& modelQueue)
	: _qmgr(qmgr)
	, _commandQ(qmgr, qmgr->commandQueue())
	, _replyQ(qmgr, modelQueue)
{
	_commandQ.open(MQOO_OUTPUT);
	_replyQ.open(MQOO_INPUT_SHARED | MQOO_FAIL_IF_QUIESCING);
}


PCF::Ptr CommandServer::createCommand(MQLONG command) const
{
	return new PCF(command, _qmgr->zos());
}

// Throws MQException
void CommandServer::sendCommand(PCF::Ptr& command, PCF::Vector& response)
{
	response.clear();

	command->message()->setReplyToQueue(_replyQ.name());
	command->message()->setExpiry(1200000);
	_commandQ.put(*command->message(), MQPMO_NO_SYNCPOINT);

	long wait = 600000;
	Message::Ptr msgResponse;

	bool keepRunning = true;
	while(keepRunning)
	{
		msgResponse = new Message(REPLY_MESSAGE_LEN);
		msgResponse->correlationId()->set(command->message()->messageId());

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
				msgResponse->correlationId()->set(command->message()->messageId());
				_replyQ.get(*msgResponse.get(), MQGMO_CONVERT | MQGMO_NO_SYNCPOINT);
			}
			else
			{
				// on z/OS we can't rely on isLast, because a response
				// can have multiple sets of responses which have a
				// last flag in each set ...
				if ( _qmgr->zos() && mqe.reason() == MQRC_NO_MSG_AVAILABLE )
				{
					keepRunning = false;
					continue;
				}

				throw;
			}
		}
		wait = 100;

		msgResponse->buffer().resize(msgResponse->dataLength());
		PCF::Ptr pcf = new PCF(msgResponse, _qmgr->zos());
		response.push_back(pcf);

		if ( ! _qmgr->zos() && pcf->isLast() )
		{
			keepRunning = false;
		}
	}
}

}
