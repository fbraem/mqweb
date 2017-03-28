/*
 * Copyright 2017 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
 * will be approved by the European Commission - subsequent
 * versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 * Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://joinup.ec.europa.eu/software/page/eupl
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the Licence is
 * distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the Licence for the specific language governing
 * permissions and limitations under the Licence.
 */

#include "MQ/Web/QueueManagerDefaultConfig.h"

namespace MQ {
namespace Web {

QueueManagerDefaultConfig::QueueManagerDefaultConfig(const std::string& qmgrName, Poco::Util::AbstractConfiguration& config)
: QueueManagerConfig(qmgrName), _config(config)
{
}

QueueManagerDefaultConfig::~QueueManagerDefaultConfig()
{
}

Poco::DynamicStruct QueueManagerDefaultConfig::read()
{
	std::string qmgrConfig = "mq.web.qmgr." + qmgrName();

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
