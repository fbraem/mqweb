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

#ifndef _MQWeb_QueueStatusController_h
#define _MQWeb_QueueStatusController_h

#include "MQ/Web/MQController.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

class QueueStatusController : public MQController
	/// Controller that shows the status of queues
{
public:
	QueueStatusController();
		/// Constructor

	virtual ~QueueStatusController();
		/// Destructor

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions

	void inquire();
		/// Action inquire. Inquire status and returns all data in JSON format.
		/// URL's:
		///  qstatus/inquire/<qmgrName>
		///  qstatus/inquire/<qmgrName>/<queueName>
		///  qstatus/inquire/<qmgrName>?queueName=MQWEB*&openType=Input
		///
		/// Query Parameters:
		///  queueName: Name of the queue (* is default).
		///  openType: All, Input or Output (default is All)
		///  statusType: Queue Status or Handle (default is Queue Status)
		///  excludeSystem: when 'true', queues starting with 'SYSTEM.' are excluded.
		///  excludeTemp: when 'true', temporary queues are excluded.
		///
		/// The queueName query parameter is ignored when a queueName is passed in the URI path.
		///
		/// The returned JSON object can contain following properties:
		///  mqweb : An object with information about the MQWeb application and request.
		///  statuses : An array with all statuses. This is always an array.
		///  When an MQ error occurs there will be no statuses property.
		///  error: An object describing the MQ error (only returned on error).

private:
};


inline const Controller::ActionMap& QueueStatusController::getActions() const
{
	static Controller::ActionMap actions 
		= MapInitializer<std::string, Controller::ActionFn>
			("inquire", static_cast<ActionFn>(&QueueStatusController::inquire))
		;
	return actions;
}


} } // Namespace MQ::Web

#endif // _MQWeb_QueueStatusController_h
