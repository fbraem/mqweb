/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
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
#include "MQ/Web/ChannelInitiatorController.h"
#include "MQ/Web/ChannelInitiatorMapper.h"

namespace MQ
{
namespace Web
{

ChannelInitiatorController::ChannelInitiatorController() : MQController()
{
}


ChannelInitiatorController::~ChannelInitiatorController()
{

}


void ChannelInitiatorController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("filter") && data().isObject("filter") )
	{
		pcfParameters = data().getObject("filter");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		set("filter", pcfParameters);

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

	}

	ChannelInitiatorMapper mapper(*commandServer(), pcfParameters);
	set("response", mapper.inquire());
}


} } // Namespace MQ::Web
