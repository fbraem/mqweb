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
#include "MQ/Web/WebController.h"
#include "MQ/Web/MultiView.h"
#include "MQ/Web/JSONView.h"

namespace MQ
{
namespace Web
{


WebController::WebController() : MQController()
{
}


WebController::~WebController()
{
}


void WebController::qmgr()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("qmgr/head.tpl"));
	multiView->add("main", new TemplateView("qmgr/index.tpl"));
	setView(multiView);
}


void WebController::queue()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("queue/head.tpl"));
	multiView->add("main", new TemplateView("queue/index.tpl"));
	setView(multiView);
}


void WebController::channel()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("channel/head.tpl"));
	multiView->add("main", new TemplateView("channel/index.tpl"));
	setView(multiView);
}


void WebController::listener()
{
	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("listener/head.tpl"));
	multiView->add("main", new TemplateView("listener/index.tpl"));
	setView(multiView);
}


void WebController::message()
{
	std::vector<std::string> parameters = getParameters();
	if ( parameters.size() < 2 )
	{
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Missing URI parameters");
		return;
	}

	mqwebData().set("queue", parameters[1]);
	std::string messageId;
	if ( parameters.size() > 2 )
	{
		messageId = parameters[2];
		mqwebData().set("messageId", messageId);
	}

	Poco::SharedPtr<MultiView> multiView = new MultiView("base.tpl");
	multiView->add("head", new TemplateView("message/head.tpl"));
	multiView->add("main", new TemplateView("message/index.tpl"));
	setView(multiView);
}

} } // Namespace MQ::Web
