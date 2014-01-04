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

	void index();
		/// Shows the index page

	void view();
		/// Shows a message

	void browse();
		/// Browse messages from a queue

	void dump();
		/// Shows the message in ascii / hex / ebcdic

	void event();
		/// Shows an event message

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available action

	std::string getDefaultAction() const;
		/// Return "index" as default action

private:


	static void mapMessageToJSON(const Message& message, Poco::JSON::Object& obj);


	static DisplayMap _reportCodes;


	static DisplayMap _messageTypeCodes;


	static DisplayMap _feedbackCodes;
};

inline const Controller::ActionMap& MessageController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("index", static_cast<ActionFn>(&MessageController::index))
			("browse", static_cast<ActionFn>(&MessageController::browse))
			("view", static_cast<ActionFn>(&MessageController::view))
			("dump", static_cast<ActionFn>(&MessageController::dump))
			("event", static_cast<ActionFn>(&MessageController::event))
		;
	return actions;
}

inline std::string MessageController::getDefaultAction() const
{
	return "index";
}


} } // Namespace MQ::Web

#endif // _MQWeb_MessageController_h
