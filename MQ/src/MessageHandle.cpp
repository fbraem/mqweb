/*
* Copyright 2019 - KBC Group NV - Franky Braem - The MIT license
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
#include <string.h>

#include <cmqc.h>

#include "Poco/Util/Application.h"

#include "MQ/Buffer.h"
#include "MQ/MQSubsystem.h"
#include "MQ/QueueManager.h"
#include "MQ/MQException.h"
#include "MQ/MessageHandle.h"

#define NAME_LENGTH 256 /* Initial name length */
#define VALUE_LENGTH 32767 /* Initial value length */

namespace MQ
{

MessageHandle::MessageHandle(Poco::SharedPtr<QueueManager> qmgr) : _qmgr(qmgr), _handle(MQHM_UNUSABLE_HMSG)
{
	MQCMHO crtMsgHOpts = { MQCMHO_DEFAULT };

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	mqSystem.functions().crtmh(_qmgr->handle(), &crtMsgHOpts, &_handle);
}

MessageHandle::~MessageHandle()
{
	if (_handle != MQHM_UNUSABLE_HMSG)
	{
		MQDMHO dltMsgHOpts = { MQDMHO_DEFAULT };
		MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
		try
		{
			mqSystem.functions().dltmh(_qmgr->handle(), &_handle, &dltMsgHOpts);
		}
		catch(MQException&)
		{
			// Don't throw exceptions from a destructor
			// See: http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.9
		}
	}
}

void MessageHandle::getProperties(Poco::DynamicStruct& properties)
{
	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	MQIMPO inqPropOpts = { MQIMPO_DEFAULT };
	inqPropOpts.Options |= MQIMPO_CONVERT_VALUE;

	Poco::Buffer<MQCHAR> nameBuffer(NAME_LENGTH);
	inqPropOpts.ReturnedName.VSPtr = nameBuffer.begin();
	inqPropOpts.ReturnedName.VSBufSize = nameBuffer.capacity();

	MQCHARV inqName = { MQPROP_INQUIRE_ALL };
	MQPD propDesc = { MQPD_DEFAULT };
	MQLONG type;
	MQLONG valueLength = VALUE_LENGTH;
	Buffer value(valueLength);
	MQLONG propsLength;

	properties.clear();

	bool ok = true;
	while(ok)
	{
		try
		{
			mqSystem.functions().inqmp(
				_qmgr->handle(),
				_handle,
				&inqPropOpts,
				&inqName,
				&propDesc,
				&type,
				valueLength,
				value.data(),
				&propsLength
			);

			std::string propertyName((char*) nameBuffer.begin(), inqPropOpts.ReturnedName.VSLength);

			switch(type)
			{
			case MQTYPE_BOOLEAN: /* Boolean value */
				properties.insert(propertyName, *(PMQBOOL) value.data() ? true : false);
				break;
			case MQTYPE_BYTE_STRING: /* Byte-string value */
				properties.insert(propertyName, value.toHex());
				break;
			case MQTYPE_FLOAT32: /* 32-bit floating-point number value */
				properties.insert(propertyName, *(PMQFLOAT32) value.data());
				break;
			case MQTYPE_FLOAT64: /* 64-bit floating-point number value */
				properties.insert(propertyName, *(PMQFLOAT64) value.data());
				break;
			case MQTYPE_INT8: /* 8-bit integer value */
				properties.insert(propertyName, *value.data());
				break;
			case MQTYPE_INT16: /* 16-bit integer value */
				properties.insert(propertyName, *(PMQINT16) value.data());
				break;
			case MQTYPE_INT32: /* 32-bit integer value */
				properties.insert(propertyName, *(PMQLONG) value.data());
				break;
			case MQTYPE_INT64: /* 64-bit integer value */
				properties.insert(propertyName, *(PMQINT64) value.data());
				break;
			case MQTYPE_NULL: /* Null value */
				//printf("NULL");
				break;
			case MQTYPE_STRING: /* String value */
			{
				std::string s((char*) value.data(), propsLength);
				properties.insert(propertyName, s);
				break;
			}
			default: /* A value with an unrecognized type */
				break;
			}

			inqPropOpts.Options = MQIMPO_CONVERT_VALUE | MQIMPO_INQ_NEXT;
		}
		catch(MQException& mqe)
		{
			switch(mqe.reason())
			{
				case MQRC_PROPERTY_NOT_AVAILABLE: // No more properties
					ok = false;
					break;
				case MQRC_PROPERTY_VALUE_TOO_BIG:
					valueLength = propsLength;
					value.resize(valueLength, false);
					inqPropOpts.Options = MQIMPO_CONVERT_VALUE | MQIMPO_INQ_PROP_UNDER_CURSOR;
					break;
				case MQRC_PROPERTY_NAME_TOO_BIG:
					nameBuffer.resize(inqPropOpts.ReturnedName.VSLength, false);
					inqPropOpts.ReturnedName.VSBufSize = inqPropOpts.ReturnedName.VSLength;
					inqPropOpts.Options = MQIMPO_CONVERT_VALUE | MQIMPO_INQ_PROP_UNDER_CURSOR;
					break;
				default:
					throw; // Other MQ error, rethrow
			}
		}
	}
}


} // Namespace MQ
