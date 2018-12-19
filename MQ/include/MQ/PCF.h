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
#ifndef _MQ_PCF_h
#define _MQ_PCF_h

#include <cmqc.h>
#include <cmqcfc.h> /* PCF  */

#include <vector>

#include "MQ/Message.h"
#include "MQ/PCFParameters.h"

namespace MQ {

class PCF
	/// Represents a Programmable Command Format (PCF) message.
{
public:
	PCF(Message::Ptr message, bool zos = false);
		/// Creates a PCF from a message

	PCF(int cmd, bool zos = false);
		/// Creates an empty PCF message for a command
		/// Use this constructor to put a PFC on a queue.

	virtual ~PCF();
		/// Destructor.

	void addParameter(MQLONG parameter, const std::string& value);
		/// Add a string parameter.

	void addParameter(MQLONG parameter, MQLONG value);
		/// Add a numeric parameter.

	void addParameter(MQLONG parameter, Buffer::Ptr buffer);
		/// Add a byte string parameter.

	void addParameterList(MQLONG parameter, MQLONG *values, unsigned int count);
		/// Add a numeric list parameter.

	void addParameterList(MQLONG parameter, const std::vector<MQLONG>& values);
		/// Add a numeric list parameter

	void addFilter(MQLONG parameter, MQLONG op, const std::string& value);
		/// Add a filter with a string value.

	void addFilter(MQLONG parameter, MQLONG op, MQLONG value);
		/// Add a filter with a numeric value.

	const PCFParameters& getParameters() const;
		/// Returns the parameters

	bool hasParameter(MQLONG id) const;
		/// Returns true when the parameter is found in the PCF message.

	int getCommand() const;
		/// Returns the command.

	int getCompletionCode() const;
		/// Returns the completion code.

	int getControl() const;
		/// Returns the PCF message sequence number

	int getMessageSequenceNumber() const;
		/// Returns the PCF message sequence number

	int getReasonCode() const;
		/// Returns the reason code.

	bool isExtendedResponse() const;
		/// Returns true when this is an extended response.

	bool isLast() const;
		/// Returns true when this PCF message is the last of a response.

	Message::Ptr message() const;


	typedef Poco::SharedPtr<PCF> Ptr;


	typedef std::vector<Ptr> Vector;


	static Ptr create(Message::Ptr message, bool zos = false);

private:

	Message::Ptr _message;

	
	bool _zos;


	PCFParameters _parameters;


	PCF(const PCF& pcf);
		/// Don't allow a copy

	void incrementParameterCount();
		/// Increment the parameter counter in the PCF header.

	friend class CommandServer;
};

inline void PCF::addParameterList(MQLONG parameter, const std::vector<MQLONG>& values)
{
	addParameterList(parameter, (MQLONG*) &values[0], (unsigned int) values.size());
}

inline int PCF::getCommand() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) _message->buffer().data();
	return header->Command;
}

inline int PCF::getCompletionCode() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) _message->buffer().data();
	return header->CompCode;
}

inline const PCFParameters& PCF::getParameters() const
{
	return _parameters;
}

inline int PCF::getMessageSequenceNumber() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*)_message->buffer().data();
	return header->MsgSeqNumber;
}

inline int PCF::getControl() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*)_message->buffer().data();
	return header->Control;
}

inline int PCF::getReasonCode() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) _message->buffer().data();
	return header->Reason;
}

inline bool PCF::hasParameter(MQLONG id) const
{
	return _parameters.has(id);
}

inline void PCF::incrementParameterCount()
{
	MQCFH* header = (MQCFH*) (MQBYTE*) _message->buffer().data();
	header->ParameterCount++;
}

inline bool PCF::isExtendedResponse() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) _message->buffer().data();
	return header->Type == MQCFT_XR_SUMMARY;
}

inline bool PCF::isLast() const
{
	MQCFH* header = (MQCFH*)(MQBYTE*) _message->buffer().data();
	return header->Control == MQCFC_LAST;
}


inline Message::Ptr PCF::message() const
{
	return _message;
}

} // MQ namespace

#endif  //_MQ_PCF_h
