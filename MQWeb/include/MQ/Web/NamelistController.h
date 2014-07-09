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

#ifndef _MQWeb_NamelistController_h
#define _MQWeb_NamelistController_h

#include "MQ/Web/MQController.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

class NamelistController : public MQController
  /// Controller that shows the details of a channel
{
public:
	NamelistController();
		/// Constructor

	virtual ~NamelistController();
		/// Destructor

	void inquire();
		/// Action inquire. Inquire the namelists and returns the details in JSON format.
		/// URL:
		///  nl/inquire/<qmgrName>
		///  nl/inquire/<qmgrName>/<namelistName>
		///  nl/inquire/<qmgrName>?namelistName=MQWEB*
		///
		/// Query Parameters:
		///  namelistName: Name of the namelist (* is default).
		///  excludeSystem: When 'true', don't return system namelists
		///
		/// Query parameters are ignored when a namelistName is passed in the URI path.
		///
		/// The returned JSON object can contain following properties:
		///  mqweb : An object with information about the MQWeb application and request.
		///  namelists : An array with all matching namelists. This is always an array (even when a namelistname is passed in the URI path).
		///   When an MQ error occurs there will be no namelists property.
		///  error: An object describing the MQ error (only returned on error).

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions.

private:
};


inline const Controller::ActionMap& NamelistController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("inquire", static_cast<ActionFn>(&NamelistController::inquire))
		;
	return actions;
}


}} // Namespace MQ::Web

#endif // _MQWeb_NamelistController_h
