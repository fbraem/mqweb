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
#include "MQ/Web/QueueMapper.h"
#include "MQ/MQException.h"

namespace MQ {
namespace Web {

QueueMapper::QueueMapper(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: MQMapper(commandServer, "Queue", input)
{
}

QueueMapper::~QueueMapper()
{
}


void QueueMapper::change()
{
	poco_assert_dbg(false); // Not yet implemented
}


void QueueMapper::create(bool replace)
{
	createCommand(MQCMD_CREATE_Q);
	fillPCF();

	PCF::Vector commandResponse;
	execute(commandResponse);

	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) 
		{
			throw MQException("PCF", "MQCMD_CREATE_Q", (*it)->getReasonCode(), (*it)->getReasonCode());
		}
	}
}


void QueueMapper::copy(bool replace)
{
	poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueMapper::inquire()
{
	createCommand(MQCMD_INQUIRE_Q);

	// Required Parameters
	addParameter<std::string>(MQCA_Q_NAME, "QName");

	// Optional Parameters
	addParameter<std::string>(MQCA_CF_STRUC_NAME, "CFStructure");

	Poco::Dynamic::Var clusterInfo = _input->get("ClusterInfo");
	if (! clusterInfo.isEmpty() )
	{
		try
		{
			pcf()->addParameter(MQIACF_CLUSTER_INFO, clusterInfo.convert<bool>() == true);
		}
		catch(...)
		{
			poco_assert_dbg(false);
		}
	}
	addParameter<std::string>(MQCA_CLUSTER_NAME, "ClusterName");
	addParameter<std::string>(MQCA_CLUSTER_NAMELIST, "ClusterNamelist");
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addParameter<MQLONG>(MQIA_PAGESET_ID, "PageSetId");
	addStringFilter();
	addParameterNumFromString(MQIA_Q_TYPE, "QType");
	addAttributeList(MQIACF_Q_ATTRS, "QAttrs");
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");

	MQLONG usage = -1;
	if ( _input->has("Usage") )
	{
		std::string usageValue = _input->optValue<std::string>("Usage", "");
		if (   Poco::icompare(usageValue, "Transmission") == 0
			|| Poco::icompare(usageValue, "XmitQ") == 0 )
		{
			usage = MQUS_TRANSMISSION;
		}
		else if ( Poco::icompare(usageValue, "Normal") == 0 )
		{
			usage = MQUS_NORMAL;
		}
	}

	PCF::Vector commandResponse;
	execute(commandResponse);

	bool excludeSystem = _input->optValue("ExcludeSystem", false);
	bool excludeTemp = _input->optValue("ExcludeTemp", false);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( usage != -1 && (*it)->hasParameter(MQIA_USAGE) )
		{
			MQLONG queueUsage = (*it)->getParameterNum(MQIA_USAGE);
			if ( queueUsage != usage )
			{
				continue;
			}
		}

		std::string qName = (*it)->getParameterString(MQCA_Q_NAME);
		if ( excludeSystem
			&& qName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		if ( excludeTemp
			&& (*it)->hasParameter(MQIA_DEFINITION_TYPE)
			&& (*it)->getParameterNum(MQIA_DEFINITION_TYPE) == MQQDT_TEMPORARY_DYNAMIC )
		{
			continue;
		}

		Poco::JSON::Object::Ptr data = new Poco::JSON::Object();
		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
