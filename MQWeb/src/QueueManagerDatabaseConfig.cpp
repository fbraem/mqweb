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
#include "MQ/Web/QueueManagerDatabaseConfig.h"

namespace MQ {
namespace Web {

using namespace Poco::Data::Keywords;

QueueManagerDatabaseConfig::QueueManagerDatabaseConfig(const std::string& qmgrName, const std::string& connector, const std::string& connectionString)
: QueueManagerConfig(qmgrName), _session(new Poco::Data::Session(connector, connectionString))
{
}

QueueManagerDatabaseConfig::~QueueManagerDatabaseConfig()
{
}

void QueueManagerDatabaseConfig::list(std::vector<std::string>& arr) const
{
	typedef Poco::Tuple<std::string> Attribute;
	std::vector<Attribute> attributes;

	*_session << "SELECT name FROM queuemanagers", into(attributes), now;
	for(std::vector<Attribute>::iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		arr.push_back(it->get<0>());
	}
}

Poco::DynamicStruct QueueManagerDatabaseConfig::read()
{
	Poco::DynamicStruct connectionInformation;

	std::string name = qmgrName();
	std::string channel;
	std::string server;
	int port = 0;

	*_session << "SELECT channel, server, port FROM queuemanagers WHERE name = :n", into(channel), into(server), into(port), useRef(name), now;

	connectionInformation.insert("channel", channel);
	connectionInformation.insert("connection", Poco::format("%s(%d)", server, port));

	return connectionInformation;
}

} } // Namespace MQ::Web
