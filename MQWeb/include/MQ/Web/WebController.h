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

#ifndef _MQWeb_WebController_h
#define _MQWeb_WebController_h

#include "MQ/Web/MQController.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

class WebController : public MQController
	/// Controller that shows the details of a queue
{
public:
	WebController();
		/// Constructor

	virtual ~WebController();
		/// Destructor

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions

	bool isJSON() const;

	void qmgr();

	void queue();

	void channel();

	void chstatus();

	void listener();

	void lstatus();

	void message();

private:
};


inline const Controller::ActionMap& WebController::getActions() const
{
	static Controller::ActionMap actions 
		= MapInitializer<std::string, Controller::ActionFn>
			("qmgr", static_cast<ActionFn>(&WebController::qmgr))
			("queue", static_cast<ActionFn>(&WebController::queue))
			("channel", static_cast<ActionFn>(&WebController::channel))
			("listener", static_cast<ActionFn>(&WebController::listener))
			("message", static_cast<ActionFn>(&WebController::message))
		;
	return actions;
}

inline bool WebController::isJSON() const
{
	return false;
}

} } // Namespace MQ::Web

#endif // _MQWeb_WebController_h
