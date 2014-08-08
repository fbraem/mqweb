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
#include "MQ/Web/QueueManagerStatusController.h"
#include "MQ/Web/QueueManagerStatusMapper.h"
#include "MQ/Web/JSONView.h"

namespace MQ
{
namespace Web
{


QueueManagerStatusController::QueueManagerStatusController() : MQController()
{
}


QueueManagerStatusController::~QueueManagerStatusController()
{
}


void QueueManagerStatusController::inquire()
{
	if ( qmgr()->zos() )
	{
		// QueueManagerStatus is not supported on z/OS
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_NOT_IMPLEMENTED, "/qmstatus/inquire not implemented for z/OS");
		return;
	}

	QueueManagerStatusMapper queueManagerStatusMapper(*commandServer());

	Poco::JSON::Object::Ptr dummyFilter = new Poco::JSON::Object();
	Poco::JSON::Array::Ptr jsonQueueManagers = queueManagerStatusMapper.inquire(dummyFilter);

	if ( jsonQueueManagers->size() > 0 )
	{
		set("status", jsonQueueManagers->get(0));
	}
}


} } // Namespace MQ::Web
