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

#ifndef _MQWeb_MQWebController_h
#define _MQWeb_MQWebController_h

#include "MQ/Web/Controller.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

class MQWebController : public Controller
	/// Controller that shows the details of MQWeb
{
public:
	MQWebController();
		/// Constructor

	virtual ~MQWebController();
		/// Destructor

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions

	void inquire();
		/// Returns MQWeb information like: version, client/binding mode, ...

	void list();
		/// Returns a list of available queuemanagers. In client mode, all queuemanagers
		/// that are configured are returned, in binding mode MQWeb will try to 
		/// execute the mq utility dspmq.

private:
};


inline const Controller::ActionMap& MQWebController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("inquire", static_cast<ActionFn>(&MQWebController::inquire))
			("list", static_cast<ActionFn>(&MQWebController::list))
		;
	return actions;
}


} } // Namespace MQ::Web

#endif // _MQWeb_MQWebController_h
