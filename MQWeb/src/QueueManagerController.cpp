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
#include <sstream>

#include <MQ/Web/QueueManagerController.h>
#include <MQ/Web/QueueManagerMapper.h>
#include <MQ/Web/MultiView.h>
#include <MQ/Web/JSONView.h>

namespace MQ
{
namespace Web
{


QueueManagerController::QueueManagerController() : MQController()
{
}


QueueManagerController::~QueueManagerController()
{
}


void QueueManagerController::index()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("qmgr/head.tpl"));
	multiView->add("main", new TemplateView("qmgr/index.tpl"));
	setView(multiView);
}


void QueueManagerController::view()
{
	QueueManagerMapper queueManagerMapper(*commandServer());

	Poco::JSON::Object::Ptr dummyFilter = new Poco::JSON::Object();
	Poco::JSON::Array::Ptr jsonQueueManagers = queueManagerMapper.inquire(dummyFilter);

	if ( jsonQueueManagers->size() > 0 )
	{
		set("qmgr", jsonQueueManagers->get(0));
		if ( format().compare("json") == 0 )
		{
			setView(new JSONView());
		}
		else
		{
			//TODO: Not allowed ...
		}
	}
}


} } // Namespace MQ::Web
