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
#include "MQ/Web/QueueController.h"
#include "MQ/Web/QueueMapper.h"
#include "MQ/Web/JSONView.h"

namespace MQ
{
namespace Web
{


QueueController::QueueController() : MQController()
{
}


QueueController::~QueueController()
{
}


void QueueController::inquire()
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
			else
			{
				queueName = form().get("QueueName", "*");
			}
			if ( queueName.empty() )
			{
				queueName = "*";
			}
			pcfParameters->set("QName", queueName);
		}

		if ( form().has("ClusterInfo") )
		{
			std::string clusterInfo = form().get("ClusterInfo");
			pcfParameters->set("ClusterInfo", Poco::icompare(clusterInfo, "true") == 0 ? "true" : "false");
		}

		if ( form().has("ClusterName") )
		{
			pcfParameters->set("ClusterName", form().get("ClusterName"));
		}

		if ( form().has("ClusterNameList") )
		{
			pcfParameters->set("ClusterNamelist", form().get("ClusterNamelist"));
		}

		if ( form().has("CommandScope") )
		{
			pcfParameters->set("CommandScope", form().get("CommandScope"));
		}

		if ( form().has("PageSetId") )
		{
			pcfParameters->set("PageSetId", form().get("PageSetId"));
		}

		if ( form().has("QSGDisposition") )
		{
			pcfParameters->set("QSGDisposition", form().get("QSGDisposition"));
		}

		std::string queueDepthField;
		if ( form().has("CurrentQDepth") )
		{
			queueDepthField = form().get("CurrentQDepth", "");
		}
		else if ( form().has("QueueDepth"))
		{
			queueDepthField = form().get("QueueDepth", "");
		}
		if ( !queueDepthField.empty() )
		{
			int queueDepth = 0;
			if ( Poco::NumberParser::tryParse(queueDepthField, queueDepth) )
			{
				Poco::JSON::Object::Ptr filter = new Poco::JSON::Object();
				filter->set("Parameter", "CurrentQDepth");
				filter->set("Operator", "NLT"); //Not Less##
				filter->set("FilterValue", queueDepth);
				pcfParameters->set("IntegerFilterCommand", filter);
			}
		}

		handleFilterForm(pcfParameters);

		if ( form().has("QueueUsage") )
		{
			pcfParameters->set("Usage", form().get("QueueUsage"));
		}
		else if ( form().has("Usage") )
		{
			pcfParameters->set("Usage", form().get("Usage"));
		}

		std::string queueType;
		if ( form().has("QueueType") )
		{
			queueType = form().get("QueueType");
		}
		else
		{
			queueType = form().get("QType", "");
		}
		if ( !queueType.empty() )
		{
			pcfParameters->set("QType", queueType);
		}

		pcfParameters->set("ExcludeSystem", form().get("excludeSystem", "false").compare("true") == 0);
		pcfParameters->set("ExcludeTemp", form().get("excludeTemp", "false").compare("true") == 0);

		Poco::JSON::Array::Ptr attrs = new Poco::JSON::Array();
		pcfParameters->set("QAttrs", attrs);

		Poco::Net::NameValueCollection::ConstIterator itAttrs = form().find("QAttrs");
		if ( itAttrs == form().end() )
		{
			itAttrs = form().find("Attrs");
		}

		for(; 
			itAttrs != form().end() && Poco::icompare(itAttrs->first, "Attrs") == 0;
			++itAttrs)
		{
			attrs->add(itAttrs->second);
		}
	}

	QueueMapper mapper(*commandServer());
	Poco::JSON::Array::Ptr queues = mapper.inquire(pcfParameters);
	set("queues", queues);
	setView(new JSONView());
}


} } // Namespace MQ::Web
