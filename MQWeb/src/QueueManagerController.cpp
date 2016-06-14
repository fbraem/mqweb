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
#include "MQ/Web/QueueManagerController.h"
#include "MQ/Web/QueueManagerMapper.h"

#include "Poco/StringTokenizer.h"

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


void QueueManagerController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("input") && data().isObject("input") )
	{
		// Posted JSON
		pcfParameters = data().getObject("input");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		meta().set("input", pcfParameters);
		if ( isJSONAPI() )
		{
			if ( form().has("fields") )
			{
				std::string fields = form().get("fields");
				Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();

				Poco::StringTokenizer tokenizer(fields, ",", Poco::StringTokenizer::TOK_IGNORE_EMPTY);
				for(Poco::StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it)
				{
					attrs->add(*it);
				}

				if ( attrs->size() > 0 )
				{
					pcfParameters->set("QMgrAttrs", attrs);
				}
			}
		}
		else
		{
			Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
			formElementToJSONArray("QMgrAttrs", attrs);
			if ( attrs->size() == 0 ) // Nothing found for QMgrAttrs, try Attrs
			{
				formElementToJSONArray("Attrs", attrs);
			}
			if ( attrs->size() > 0 )
			{
				pcfParameters->set("QMgrAttrs", attrs);
			}

			if ( form().has("CommandScope") )
			{
				pcfParameters->set("CommandScope", form().get("CommandScope"));
			}
		}
	}

	QueueManagerMapper mapper(*commandServer(), pcfParameters);
	Poco::JSON::Array::Ptr json = mapper.inquire();
	if ( json->size() > 0 )
	{
		Poco::JSON::Object::Ptr data = new Poco::JSON::Object();
		set("data", json->get(0));
	}
}


} } // Namespace MQ::Web
