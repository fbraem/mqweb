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
#ifndef _MQWeb_MessageController_h
#define _MQWeb_MessageController_h

#include "MQ/Web/MQController.h"
#include "MQ/Web/Dictionary.h"

namespace MQ
{
namespace Web
{

class MessageController : public MQController
	/// Controller for handling Websphere MQ messages.
{
public:
	MessageController();
		/// Constructor

	virtual ~MessageController();
		/// Destructor

	void browse();
		/// Action browse.

	void dump();
		/// Action dump. Returns the message in ascii / hex / ebcdic in JSON format
		/// URL':
		///  /message/dump/<qmgrName>/<queuName>/<msgId>
		///
		/// Currently the message size is restricted to 16K.

	void publish();
		/// Publish a message to a topic
		///   /message/publish/<qmgrName>/<topicName>/<topicString>

	void put();
		/// Put a message on a queue
		///   /message/put/<qmgrName>/<queueName>

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available action

private:


	static void mapMessageToJSON(const Message& message, Poco::JSON::Object& obj);


	static void mapJSONToMessage(const Poco::JSON::Object& obj, Message& message);


	static Dictionary _reportCodes;


	static Dictionary _messageTypeCodes;


	static Dictionary _feedbackCodes;
};

inline const Controller::ActionMap& MessageController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("browse", static_cast<ActionFn>(&MessageController::browse))
			("dump", static_cast<ActionFn>(&MessageController::dump))
			("publish", static_cast<ActionFn>(&MessageController::publish))
			("put", static_cast<ActionFn>(&MessageController::put))
		;
	return actions;
}

} } // Namespace MQ::Web

#endif // _MQWeb_MessageController_h
