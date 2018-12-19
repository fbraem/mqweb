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
#include "MQ/Web/ConnectionInquire.h"

#include "Poco/HexBinaryDecoder.h"

namespace MQ {
namespace Web {


ConnectionInquire::ConnectionInquire(CommandServer& commandServer, Poco::JSON::Object::Ptr input)
: PCFCommand(commandServer, MQCMD_INQUIRE_CONNECTION, "Connection", input)
{
	// Required parameters
	if ( input->has("ConnectionId") )
	{
		std::string hexId = input->get("ConnectionId");
		if ( hexId.length() > MQ_CONNECTION_ID_LENGTH )
		{
			hexId.erase(MQ_CONNECTION_ID_LENGTH);
		}
		Buffer::Ptr id = new Buffer(hexId);

		pcf()->addParameter(MQBACF_CONNECTION_ID, id);
	}
	else
	{
		Buffer::Ptr id = new Buffer(MQ_CONNECTION_ID_LENGTH); // Empty buffer
		memset(id->data(), 0, MQ_CONNECTION_ID_LENGTH);
		pcf()->addParameter(MQBACF_GENERIC_CONNECTION_ID, id);
	}

	// Optional parameters
	//TODO: ByteStringFilter
	addParameter<std::string>(MQCACF_COMMAND_SCOPE, "CommandScope");
	addAttributeList(MQIACF_CONNECTION_ATTRS, "ConnectionAttrs");
	addParameterNumFromString(MQIACF_CONN_INFO_TYPE, "ConnInfoType");
	addIntegerFilter();
	addStringFilter();
	addParameterNumFromString(MQIA_UR_DISP, "URDisposition");

	if ( ! input->has("ConnectionAttrs") )
	{
		// It seems that this is not set by default, so we do
		// it ourselves.
		MQLONG attrs[] = { MQIACF_ALL };
		pcf()->addParameterList(MQIACF_CONNECTION_ATTRS, attrs, 1);
	}
}

ConnectionInquire::~ConnectionInquire()
{
}

Poco::JSON::Array::Ptr ConnectionInquire::execute()
{
	PCFCommand::execute();

	Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
	for(PCF::Vector::const_iterator it = begin(); it != end(); it++)
	{
		if ( (*it)->getReasonCode() != MQRC_NONE ) // Skip errors (2035 not authorized for example)
			continue;

		if ( (*it)->isExtendedResponse() ) // Skip extended response
			continue;

		json->add(createJSON(**it));
	}

	return json;
}

}} //  Namespace MQ::Web
