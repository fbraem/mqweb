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
#ifndef _MQ_QueueManagerDatabaseConfig_h
#define _MQ_QueueManagerDatabaseConfig_h

#include "MQ/Web/QueueManagerConfig.h"

#include "Poco/Data/Session.h"

namespace MQ {
namespace Web {

class QueueManagerDatabaseConfig : public QueueManagerConfig
	/// Class that retrieves queuemanager configuration from a database.
{
public:

	QueueManagerDatabaseConfig(const std::string& qmgrName, const std::string& connector, const std::string& connectionString, const std::string& tableName);
	/// Constructor.

	QueueManagerDatabaseConfig(const std::string& qmgrName, const std::string& connector, const std::string& connectionString);
		/// Constructor.

	virtual ~QueueManagerDatabaseConfig();
		/// Destructor.

	void list(std::vector<std::string>& arr) const;
		/// Returns all configured queuemanagers

	Poco::DynamicStruct read();
		/// Read the configuration

private:

	mutable Poco::SharedPtr<Poco::Data::Session> _session;

	std::string _tableName;
};

}} // Namespace MQ::Web

#endif // _MQ_QueueManagerDatabaseConfig_h
