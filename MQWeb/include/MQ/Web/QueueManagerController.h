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

#ifndef _MQWeb_QueueManagerController_h
#define _MQWeb_QueueManagerController_h

#include "MQ/Web/MQController.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ
{
namespace Web
{


class QueueManagerController : public MQController
	/// Controller for a QueueManager object
{
public:
	QueueManagerController();
		/// Constructor

	virtual ~QueueManagerController();
		/// Destructor

	const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions

	void inquire();
		/// Action inquire. Inquire the queuemanager and returns all data in JSON format.
		/// URL's:
		///  qmgr/inquire/<qmgrName>
		///
		/// The returned JSON object can contain following properties:
		///  mqweb : An object with information about the MQWeb application and request.
		///  qmgr : An object with all queuemanager details.
		///  error: An object describing the MQ error (only returned on error).
};

inline const Controller::ActionMap& QueueManagerController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("inquire", static_cast<ActionFn>(&QueueManagerController::inquire))
		;
	return actions;
}


}} // Namespace MQ::Web

#endif // _MQWeb_QueueManagerController_h
