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
		///   /message/publish/<qmgrName>/<queueName>

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
