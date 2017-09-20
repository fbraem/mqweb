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
#ifndef _MQ_CommandServer_h
#define _MQ_CommandServer_h

#include <cmqc.h>
#include <vector>

#include "MQ/Queue.h"
#include "MQ/PCF.h"

#include "Poco/SharedPtr.h"

namespace MQ {

class QueueManager;

class CommandServer
	/// Class for sending PCF commands to a queuemanager
{
public:
	std::string commandQName() const;
		/// Returns the name of the command queue.

	PCF::Ptr createCommand(MQLONG command) const;
		/// Returns a shared pointer to a PCF object for the given command.

	void sendCommand(PCF::Ptr& command, PCF::Vector& response);
		/// Sends the command to the queuemanager. The response is returned
		/// as a vector of PCF objects. Can throw a MQException.

	std::string replyQName() const;
		/// Returns the name of the reply queue.

private:
	CommandServer(QueueManager& qmgr, const std::string& modelQueue);
		/// Constructor.

	CommandServer(const CommandServer& copy);
	CommandServer& operator = (const  CommandServer& copy);

	QueueManager& _qmgr;

	Queue _commandQ;

	Queue _replyQ;

	friend class QueueManager;
};

inline std::string CommandServer::commandQName() const
{
	return _commandQ.name();
}

inline std::string CommandServer::replyQName() const
{
	return _replyQ.name();
}

} // namespace MQ

#endif //  _MQ_CommandServer_h
