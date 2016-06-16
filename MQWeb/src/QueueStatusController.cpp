/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
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
#include "MQ/Web/QueueStatusController.h"
#include "MQ/Web/QueueStatusMapper.h"

namespace MQ
{
namespace Web
{


QueueStatusController::QueueStatusController() : MQController()
{
}


QueueStatusController::~QueueStatusController()
{
}


void QueueStatusController::inquire()
{
	Poco::JSON::Object::Ptr pcfParameters;

	if ( data().has("input") && data().isObject("input") )
	{
		pcfParameters = data().getObject("input");
	}
	else
	{
		pcfParameters = new Poco::JSON::Object();
		set("input", pcfParameters);

		std::vector<std::string> parameters = getParameters();
		// First parameter is queuemanager
		// Second parameter can be a queuename. If this is passed, the
		// query parameter QName or queueName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("QName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			std::string queueName;
			if ( form().has("QName") )
			{
				queueName = form().get("QName");
			}
			else if ( form().has("QueueName") )
			{
				queueName = form().get("QueueName");
			}
			else if ( form().has("name") )
			{
				queueName = form().get("name");
			}

			if ( queueName.empty() )
			{
				queueName = "*";
			}
			pcfParameters->set("QName", queueName);
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);
		pcfParameters->set("ExcludeTemp", form().get("ExcludeTemp", "false").compare("true") == 0);

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}
		if ( form().has("QSGDisposition") )
		{
			pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
		}

		if ( form().has("StatusType") ) pcfParameters->set("StatusType", form().get("StatusType"));
		if ( form().has("OpenType") ) pcfParameters->set("OpenType", form().get("OpenType"));

		handleFilterForm(pcfParameters);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("QStatusAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for QStatusAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("QStatusAttrs", attrs);
		}

	}

	QueueStatusMapper mapper(*commandServer(), pcfParameters);
	set("data", mapper.inquire());
}


} } // Namespace MQ::Web
