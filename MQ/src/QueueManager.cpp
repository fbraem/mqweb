/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they
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
#include <string.h>

#include "Poco/DateTimeParser.h"
#include "Poco/Util/Application.h"

#include "MQ/QueueManager.h"
#include "MQ/MQException.h"
#include "MQ/MQSubsystem.h"

namespace MQ
{
QueueManager::QueueManager(const std::string& name)
:	 _handle(0),
	_name(name),
	_applicationType(0)
{
}


QueueManager::~QueueManager()
{
	if ( _handle != 0 )
	{
		try
		{
			disconnect();
		}
		catch(MQException&)
		{
			// Don't throw exceptions from a destructor
			// See: http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.9
		}
	}
}

void QueueManager::connect()
{
	MQCNO cno = { MQCNO_DEFAULT };
	cno.Version = MQCNO_VERSION_2;
	//cno.Options = MQCNO_HANDLE_SHARE_NO_BLOCK;
	cno.Options = MQCNO_HANDLE_SHARE_NONE;

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	_handle = mqSystem.functions().connx(_name, &cno);

	inquireQmgrAttrs();
}

void QueueManager::connect(const std::string& channel, const std::string& server)
{
	MQCNO cno = { MQCNO_DEFAULT };
	cno.Version = MQCNO_VERSION_2;
	//cno.Options = MQCNO_HANDLE_SHARE_NO_BLOCK;
	cno.Options = MQCNO_HANDLE_SHARE_NONE;

	MQCD cd = { MQCD_CLIENT_CONN_DEFAULT };
	strncpy(cd.ChannelName, channel.c_str(), MQ_CHANNEL_NAME_LENGTH);
	strncpy(cd.ConnectionName, server.c_str(), MQ_CONN_NAME_LENGTH);
	cd.TransportType = MQXPT_TCP;
	cno.ClientConnPtr = &cd;

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	_handle = mqSystem.functions().connx(_name, &cno);

	inquireQmgrAttrs();
}

void QueueManager::inquireQmgrAttrs()
{
	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();

	MQOD qmgrObjectDesc = {MQOD_DEFAULT};
	qmgrObjectDesc.ObjectType = MQOT_Q_MGR;
	MQHOBJ qmgrObjectHandle = mqSystem.functions().open(_handle, &qmgrObjectDesc, MQOO_INQUIRE);

	std::vector<int> intSelectors;
	intSelectors.push_back(MQIA_PLATFORM);

	std::map<int, int> charSelectors;
	charSelectors[MQCA_Q_MGR_NAME] = MQ_Q_MGR_NAME_LENGTH;
	charSelectors[MQCA_Q_MGR_IDENTIFIER] = MQ_Q_MGR_IDENTIFIER_LENGTH;
	charSelectors[MQCA_COMMAND_INPUT_Q_NAME] = MQ_Q_NAME_LENGTH;

	std::map<int, int> intResult;
	std::map<int, std::string> charResult;
	mqSystem.functions().inq(_handle, qmgrObjectHandle, intSelectors, charSelectors, intResult, charResult);

	_applicationType = intResult[MQIA_PLATFORM];
	_name = charResult[MQCA_Q_MGR_NAME];
	_id = charResult[MQCA_Q_MGR_IDENTIFIER];
	_commandQueue = charResult[MQCA_COMMAND_INPUT_Q_NAME];

	mqSystem.functions().close(_handle, &qmgrObjectHandle, MQCO_NONE);
}

void QueueManager::disconnect()
{
	if ( _handle != 0 )
	{
		MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
		mqSystem.functions().disc(&_handle);
		_handle = 0;
	}
}

}
