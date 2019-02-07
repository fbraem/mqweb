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
#include "MQ/Web/QueueManagerDefaultConfig.h"

namespace MQ {
namespace Web {

QueueManagerDefaultConfig::QueueManagerDefaultConfig(Poco::Util::AbstractConfiguration& config)
: QueueManagerConfig(), _config(config)
{
}

QueueManagerDefaultConfig::~QueueManagerDefaultConfig()
{
}

void QueueManagerDefaultConfig::list(std::vector<std::string>& arr) const
{
	Poco::Util::AbstractConfiguration::Keys qmgrs;
	_config.keys("mq.web.qmgr", qmgrs);

	for(Poco::Util::AbstractConfiguration::Keys::iterator it = qmgrs.begin(); it != qmgrs.end(); ++it)
	{
		arr.push_back(*it);
	}
}

Poco::DynamicStruct QueueManagerDefaultConfig::read(const std::string& qmgrName) const
{
	std::string qmgrConfig = "mq.web.qmgr." + qmgrName;

	Poco::DynamicStruct connectionInformation;
	std::string qmgrConfigConnection = qmgrConfig + ".connection";
	if ( _config.has(qmgrConfigConnection) )
	{
		std::string connection;
		std::string channel;
		connectionInformation.insert("connection", _config.getString(qmgrConfigConnection));
		std::string qmgrConfigChannel = qmgrConfig + ".channel";
		if ( _config.has(qmgrConfigChannel) )
		{
			connectionInformation.insert("channel", _config.getString(qmgrConfigChannel));
		}
		else
		{
			connectionInformation.insert("channel", _config.getString("mq.web.defaultChannel", "SYSTEM.DEF.SVRCONN"));
		}

		std::string qmgrConfigUser = qmgrConfig + ".user";
		if ( _config.has(qmgrConfigUser) )
		{
			connectionInformation.insert("user", _config.getString(qmgrConfigUser));
			connectionInformation.insert("pwd", _config.getString(qmgrConfig + ".pwd", ""));
		} else {
			std::string globalUser = "mq.web.defaultUser";
			if (_config.has("mq.web.defaultUser"))
			{
				connectionInformation.insert("user", _config.getString("mq.web.defaultUser"));
				connectionInformation.insert("pwd", _config.getString("mq.web.defaultPwd", ""));
			}
		}

		if ( _config.has("mq.web.ssl.keyrepos") )
		{
			Poco::DynamicStruct ssl;
			connectionInformation.insert("ssl", ssl);

			Poco::Util::AbstractConfiguration::Keys keys;
			Poco::AutoPtr<Poco::Util::AbstractConfiguration> sslConfig = _config.createView("mq.web.ssl");
			sslConfig->keys(keys);
			for(Poco::Util::AbstractConfiguration::Keys::iterator it = keys.begin(); it != keys.end(); ++it)
			{
				ssl.insert(*it, _config.getString(*it));
			}
		}
	}

	return connectionInformation;
}

} } // Namespace MQ::Web
