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
