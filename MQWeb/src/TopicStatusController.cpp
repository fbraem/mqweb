/*
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
#include "MQ/Web/TopicStatusController.h"
#include "MQ/Web/TopicStatusMapper.h"

namespace MQ
{
namespace Web
{

TopicStatusController::TopicStatusController() : MQController()
{
}


TopicStatusController::~TopicStatusController()
{

}


void TopicStatusController::inquire()
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
		// Second parameter can be a topicstring. If this is passed
		// the query parameter topicstring is ignored.
		if ( parameters.size() > 1 )
		{
			pcfParameters->set("TopicString", parameters[1]);
		}
		else
		{
			// Handle query parameters
			pcfParameters->set("TopicString", form().get("TopicString", "*"));
		}

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		formElementToJSONArray("TopicStatusAttrs", attrs);
		if ( attrs->size() == 0 ) // Nothing found for TopicStatusAttrs, try Attrs
		{
			formElementToJSONArray("Attrs", attrs);
		}
		if ( attrs->size() > 0 )
		{
			pcfParameters->set("TopicStatusAttrs", attrs);
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("StatusType") )
		{
			pcfParameters->set("StatusType", form().get("StatusType"));
		}

		handleFilterForm(pcfParameters);
	}

	TopicStatusMapper mapper(*commandServer(), pcfParameters);
	set("data", mapper.inquire());
}


} } // Namespace MQ::Web
