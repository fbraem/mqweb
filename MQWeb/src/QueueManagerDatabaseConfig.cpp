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

QueueManagerDatabaseConfig::QueueManagerDatabaseConfig(const std::string& connector, const std::string& connectionString, const std::string& tableName)
	: _session(new Poco::Data::Session(connector, connectionString)), _tableName(tableName)
{
}

QueueManagerDatabaseConfig::QueueManagerDatabaseConfig(const std::string& connector, const std::string& connectionString)
	: _session(new Poco::Data::Session(connector, connectionString)), _tableName("queuemanagers")
{
}

QueueManagerDatabaseConfig::~QueueManagerDatabaseConfig()
{
}


std::map<std::string, Poco::DynamicStruct> QueueManagerDatabaseConfig::read() const
{
	std::map<std::string, Poco::DynamicStruct> result;

	typedef Poco::Tuple<std::string, std::string, std::string, int> Attribute;
	std::vector<Attribute> attributes;

	*_session << "SELECT name, channel, server, port FROM " + _tableName, into(attributes), now;
	for (std::vector<Attribute>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
		Poco::DynamicStruct connectionInformation;
		connectionInformation.insert("channel", it->get<1>());
		connectionInformation.insert("connection", Poco::format("%s(%d)", it->get<2>(), it->get<3>()));
		result.insert(std::make_pair(it->get<0>(), connectionInformation));
	}

	return result;
}

} } // Namespace MQ::Web
