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

#ifndef _MQWeb_ListenerController_h
#define _MQWeb_ListenerController_h

#include "MQ/Web/MQController.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

class ListenerController : public MQController
	/// Controller that shows the details of a queue
{
public:
	ListenerController();
		/// Constructor

	virtual ~ListenerController();
		/// Destructor

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions

	std::string getDefaultAction() const;
		/// Returns index as default action

	void index();
		/// Action index. Returns a single page application (SPA) for listeners.
		/// URL's:
		///   listener/index/<qmgrName>
		///   listener/index/<qmgrName>/#<listenerName>

	void inquire();
		/// Action inquire. Inquire listeners and returns all data in JSON format.
		/// URL's:
		///  listener/inquire/<qmgrName>
		///  listener/inquire/<qmgrName>/<listenerName>
		///  listener/inquire/<qmgrName>?listenerName=MQWEB*
		///
		/// Query Parameters:
		///  listenerName: Name of the listener (* is default).
		///  type: listener type. Possible values: All, LU62, NETBIOS, SPX or TCP (default is All)
		///
		/// Query parameters are ignored when a listenerName is passed in the URI path.
		///
		/// The returned JSON object can contain following properties:
		///  mqweb : An object with information about the MQWeb application and request.
		///  listeners : An array with all matching listeners. This is always an array (even when a listenername is passed in the URI path).
		///   When an MQ error occurs there will be no listeners property.
		///  error: An object describing the MQ error (only returned on error).

private:
};


inline const Controller::ActionMap& ListenerController::getActions() const
{
	static Controller::ActionMap actions 
		= MapInitializer<std::string, Controller::ActionFn>
			("index", static_cast<ActionFn>(&ListenerController::index))
			("inquire", static_cast<ActionFn>(&ListenerController::inquire));
	return actions;
}

inline std::string ListenerController::getDefaultAction() const
{
	return "index";
}


} } // Namespace MQ::Web

#endif // _MQWeb_ListenerController_h
