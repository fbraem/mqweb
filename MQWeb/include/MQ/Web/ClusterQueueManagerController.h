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

#ifndef _MQWeb_ClusterQueueManagerController_h
#define _MQWeb_ClusterQueueManagerController_h

#include "MQ/Web/MQController.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

class ClusterQueueManagerController : public MQController
  /// Controller that shows the details of a channel
{
public:
	ClusterQueueManagerController();
		/// Constructor

	virtual ~ClusterQueueManagerController();
		/// Destructor

	void inquire();
		/// Action inquire. Inquire the the attributes of queue managers in a cluster.
		/// URL:
		///  clusqmgr/inquire/<qmgrName>
		///  clusqmgr/inquire/<qmgrName>/<clusterName>
		///  clusqmgr/inquire/<qmgrName>?clusterName=BIRDS
		///
		/// Query Parameters:
		///  clusterQmgrName: Queuemanager name. Generic queue manager names are supported. (* is default)
		///  channelName: Specifies that eligible cluster queue managers are limited to those
		///    having the specified channel name. Generic channel names are supported (* is default).
		///  clusterName: Specifies that eligible cluster queue managers are limited to those
		///    having the specified cluster name. Generic cluster names are supported (* is the default).
		///
		/// Query parameters are ignored when a clusterName is passed in the URI path.
		///
		/// The returned JSON object can contain following properties:
		///  mqweb : An object with information about the MQWeb application and request.
		///  clusqmgrs : An array with all mcluster. This is always an array (even when a clustername is passed in the URI path).
		///   When an MQ error occurs there will be no clusqmgrs property.
		///  error: An object describing the MQ error (only returned on error).

	void suspend();

	void resume();

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions.

private:
};


inline const Controller::ActionMap& ClusterQueueManagerController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("inquire", static_cast<ActionFn>(&ClusterQueueManagerController::inquire))
			("suspend", static_cast<ActionFn>(&ClusterQueueManagerController::suspend))
			("resume", static_cast<ActionFn>(&ClusterQueueManagerController::resume))
		;
	return actions;
}


}} // Namespace MQ::Web

#endif // _MQWeb_ClusterQueueManagerController_h
