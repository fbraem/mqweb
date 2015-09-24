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
#include "MQ/Web/TopicController.h"
#include "MQ/Web/TopicMapper.h"

namespace MQ
{
namespace Web
{

TopicController::TopicController() : MQController()
{
}


TopicController::~TopicController()
{

}


void TopicController::inquire()
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

		std::vector<std::string> parameters = getParameters();
		// First parameter is queuemanager
		// Second parameter can be a topicname. If this is passed
		// the query parameter topicName is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("TopicName", parameters[1]);
		}
		else
		{
			// Handle query parameters
			std::string topicNameField;
			if ( form().has("TopicName") )
			{
				topicNameField = form().get("TopicName");
			}
			else if ( form().has("name") )
			{
				topicNameField = form().get("name");
			}
			if ( topicNameField.empty() )
			{
				topicNameField = "*";
			}
			pcfParameters->set("TopicName", topicNameField);
		}

		pcfParameters->set("ExcludeSystem", form().get("ExcludeSystem", "false").compare("true") == 0);

		if ( form().has("ClusterInfo") )
		{
			std::string clusterInfo = form().get("ClusterInfo");
			pcfParameters->set("ClusterInfo", Poco::icompare(clusterInfo, "true") == 0 ? "true" : "false");
		}

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("TopicAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for TopicAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("TopicAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("QSGDisposition") )
		{
			pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
		}

		if ( form().has("TopicType") )
		{
			pcfParameters->set("TopicType", form().get("TopicType"));
		}

		handleFilterForm(pcfParameters);
	}

	TopicMapper mapper(*commandServer(), pcfParameters);
	set("topics", mapper.inquire());
}


} } // Namespace MQ::Web
