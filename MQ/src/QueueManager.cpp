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
#include "Poco/StringTokenizer.h"

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
	cno.Options = MQCNO_HANDLE_SHARE_NONE;

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	_handle = mqSystem.functions().connx(_name, &cno);

	inquireQmgrAttrs();
}

void QueueManager::connect(const std::string& channel, const std::string& server)
{
	MQCNO cno = { MQCNO_DEFAULT };
	cno.Version = MQCNO_VERSION_2;
	cno.Options = MQCNO_HANDLE_SHARE_BLOCK;

	MQCD cd = { MQCD_CLIENT_CONN_DEFAULT };
	strncpy(cd.ChannelName, channel.c_str(), MQ_CHANNEL_NAME_LENGTH);
	strncpy(cd.ConnectionName, server.c_str(), MQ_CONN_NAME_LENGTH);
	cd.TransportType = MQXPT_TCP;
	cno.ClientConnPtr = &cd;

	MQ::MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQ::MQSubsystem>();
	_handle = mqSystem.functions().connx(_name, &cno);

	inquireQmgrAttrs();
}


void QueueManager::connect(const Poco::Dynamic::Struct<std::string>& connectionInformation)
{
	Poco::Dynamic::Var ssl  = connectionInformation["ssl"];
	if ( ssl.isEmpty() )
	{
		connect(connectionInformation["channel"], connectionInformation["connection"]);
	}
	else
	{
		if ( ssl.isStruct() )
		{
			connect(connectionInformation["channel"],
				connectionInformation["connection"],
				ssl.extract<Poco::Dynamic::Struct<std::string> >());
		}
	}
}

void QueueManager::connect(const std::string& channel, const std::string& server, const Poco::Dynamic::Struct<std::string>& ssl)
{
	MQCNO cno = { MQCNO_DEFAULT };

	/*
	We must specify MQCNO version 4 to ensure that both the client 
	connection pointer and SSL configuration options are used 
	*/
	cno.Version = MQCNO_VERSION_4;
	cno.Options = MQCNO_HANDLE_SHARE_BLOCK;

	MQCD cd = { MQCD_CLIENT_CONN_DEFAULT };
	strncpy(cd.ChannelName, channel.c_str(), MQ_CHANNEL_NAME_LENGTH);
	strncpy(cd.ConnectionName, server.c_str(), MQ_CONN_NAME_LENGTH);

	Poco::Dynamic::Var cipherspec = ssl["cipherspec"];
	if ( !cipherspec.isEmpty() )
	{
		strncpy(cd.SSLCipherSpec, cipherspec.toString().c_str(), MQ_SSL_CIPHER_SPEC_LENGTH);
		cd.Version = MQCD_VERSION_7; // SSL requires MQCD version 7 or later
	}
	cd.TransportType = MQXPT_TCP;
	cno.ClientConnPtr = &cd;

	MQSCO sco = { MQSCO_DEFAULT };
	MQAIR authInfoRec = { MQAIR_DEFAULT };

	Poco::Dynamic::Var keyrepos = ssl["keyrepos"];
	strncpy(sco.KeyRepository, keyrepos.toString().c_str(), MQ_SSL_KEY_REPOSITORY_LENGTH);

	Poco::Dynamic::Var fips = ssl["fips"];
	if ( !fips.isEmpty() && fips.convert<bool>() )
	{
		sco.FipsRequired = MQSSL_FIPS_YES;
		sco.Version = MQSCO_VERSION_2; // A version 2 MQSCO supports FipsRequired
	}

	Poco::Dynamic::Var suiteb = ssl["suiteb"];
	if ( ! suiteb.isEmpty() )
	{
		static std::map<std::string, MQLONG> suiteBTable;
		if ( suiteBTable.size() == 0 )
		{
			suiteBTable.insert(std::make_pair<std::string, MQLONG>("none", MQ_SUITE_B_NONE));
			suiteBTable.insert(std::make_pair<std::string, MQLONG>("128 bit", MQ_SUITE_B_128_BIT));
			suiteBTable.insert(std::make_pair<std::string, MQLONG>("192 bit", MQ_SUITE_B_192_BIT));
		}

		sco.Version = MQSCO_VERSION_3; /* A version 3 MQSCO supports Suite B encryption policy */

		Poco::StringTokenizer tokenizer(suiteb.toString(), ",", Poco::StringTokenizer::TOK_TRIM);
		int n = 0;
		for(Poco::StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end() && n < 4; ++it, ++n)
		{
			std::map<std::string, MQLONG>::iterator suiteBIterator = suiteBTable.find(*it);
			if ( suiteBIterator == suiteBTable.end() )
			{
				throw Poco::NotFoundException(Poco::format("Unknown SuiteB value: %s", *it));
			}
			else
			{
				sco.EncryptionPolicySuiteB[n] = suiteBIterator->second;
			}
		}
	}

#ifdef MQSCO_VERSION_4
	Poco::Dynamic::Var certValPolicy = ssl["certificate_validation_policy"];
	if ( !certValPolicy.isEmpty() )
	{
		static std::map<std::string, MQLONG> certValPolicyTable;
		if ( certValPolicyTable.size() == 0 )
		{
			certValPolicyTable.insert(std::make_pair<std::string, MQLONG>("any", MQ_CERT_VAL_POLICY_ANY));
			certValPolicyTable.insert(std::make_pair<std::string, MQLONG>("rfc5280", MQ_CERT_VAL_POLICY_RFC5280));
		}

		sco.Version = MQSCO_VERSION_4; /* A version 4 MQSCO supports certificate validation policy */

		std::map<std::string, MQLONG>::iterator certValPolicyIterator = certValPolicyTable.find(certValPolicy.toString());
		if ( certValPolicyIterator == certValPolicyTable.end() )
		{
			throw Poco::NotFoundException(Poco::format("Unknown Certification Validation Policy: %s", certValPolicy));
		}
		else
		{
			sco.CertificateValPolicy = certValPolicyIterator->second;
		}
	}
#endif
	Poco::Dynamic::Var ocsp = ssl["ocsp_url"];
	if ( !ocsp.isEmpty() )
	{
		/* OCSP requires MQAIR version 2 or later */
		authInfoRec.Version = MQAIR_VERSION_2;
		authInfoRec.AuthInfoType = MQAIT_OCSP;

		strncpy(authInfoRec.OCSPResponderURL, ocsp.toString().c_str(), MQ_AUTH_INFO_OCSP_URL_LENGTH);

		/* The MQSCO must point to the MQAIR */
		sco.AuthInfoRecCount = 1;
		sco.AuthInfoRecPtr = &authInfoRec;
	}

	cno.SSLConfigPtr = &sco;

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
