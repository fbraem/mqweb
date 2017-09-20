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
#ifndef _MQ_QueueManagerConfig_h
#define _MQ_QueueManagerConfig_h

#include <vector>

#include "Poco/Dynamic/Struct.h"

namespace MQ {
namespace Web {

class QueueManagerConfig
	/// Base class for queuemanager configuration.
{
public:

	QueueManagerConfig(const std::string& qmgrName);
		/// Constructor.

	virtual ~QueueManagerConfig();
		/// Destructor.

	std::string qmgrName() const;
		/// Returns the queuemanager name

	virtual void list(std::vector<std::string>& arr) const = 0;
		/// Returns all configured queuemanagers

	virtual Poco::DynamicStruct read() = 0;
		/// Read the configuration

private:

	std::string _qmgrName;
};

inline std::string QueueManagerConfig::qmgrName() const
{
	return _qmgrName;
}

}} // Namespace MQ::Web

#endif // _MQ_QueueManagerConfig_h
