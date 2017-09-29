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
#include "MQ/Web/NamelistInquire.h"

namespace MQ {
namespace Web {


NamelistInquire::NamelistInquire(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: PCFCommand(commandServer, MQCMD_INQUIRE_NAMELIST, "Namelist", input), _excludeSystem(false)
{
	// Required parameters
	addParameter<std::string>(MQCA_NAMELIST_NAME, "NamelistName");

	// Optional parameters
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addIntegerFilter();
	addAttributeList(MQIACF_NAMELIST_ATTRS, "NamelistAttrs");
	addParameterNumFromString(MQIA_NAMELIST_TYPE, "NamelistType");
	addParameterNumFromString(MQIA_QSG_DISP, "QSGDisposition");
	addStringFilter();

	_excludeSystem = input->optValue("ExcludeSystem", false);

}

NamelistInquire::~NamelistInquire()
{
}


Poco::JSON::Array::Ptr NamelistInquire::execute()
{
	PCF::Vector commandResponse;
	PCFCommand::execute(commandResponse);

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::iterator it = commandResponse.begin(); it != commandResponse.end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		std::string namelistName = (*it)->getParameterString(MQCA_NAMELIST_NAME);
		if (   _excludeSystem
			&& namelistName.compare(0, 7, "SYSTEM.") == 0 )
		{
			continue;
		}

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
