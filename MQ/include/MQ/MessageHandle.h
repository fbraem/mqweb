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
#ifndef _MQ_MessageHandle_H
#define _MQ_MessageHandle_H

#include <cmqc.h>

#include "Poco/SharedPtr.h"
#include "Poco/Dynamic/Struct.h"

namespace MQ
{
class QueueManager;

class MessageHandle
	/// Represents a Websphere MQ queue
{
public:
	typedef Poco::SharedPtr<MessageHandle> Ptr;

	MessageHandle(Poco::SharedPtr<QueueManager> qmgr);
		/// Constructor.

	virtual ~MessageHandle();
		/// Destructor.

	void getProperties(Poco::DynamicStruct& properties);

	MQHMSG handle() const;

private:

	Poco::SharedPtr<QueueManager> _qmgr;

	MQHMSG _handle;
};

inline MQHMSG MessageHandle::handle() const
{
	return _handle;
}

}
#endif // _MQ_MessageHandle_H
