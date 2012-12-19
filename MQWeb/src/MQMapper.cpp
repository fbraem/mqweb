/*
 * Copyright 2010 MQWeb - Franky Braem
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
#include <MQ/Web/MQMapper.h>

namespace MQ {
namespace Web {

Dictionary MQMapper::_dictionary = Dictionary()
	// Common properties
	(MQCA_ALTERATION_DATE, "AlterationDate")
	(MQCA_ALTERATION_TIME, "AlterationTime")
	(MQCA_CLUSTER_NAME, "ClusterName")
	(MQCA_CLUSTER_NAMELIST, "ClusterNamelist")
	(MQIA_QSG_DISP, "QSGDisposition", DisplayMapInitializer
		(MQQSGD_COPY, "Copy")
		(MQQSGD_GROUP, "Group")
		(MQQSGD_Q_MGR, "Qmgr")
		(MQQSGD_SHARED, "Shared")
	)
	(MQCA_Q_MGR_NAME, "QMgrName")
	
	// QUEUE properties
	(MQCA_BACKOUT_REQ_Q_NAME, "BackoutRequeueName")
	(MQIA_BACKOUT_THRESHOLD, "BackoutThreshold")
	(MQCA_BASE_Q_NAME, "BaseQName")
	(MQCA_CF_STRUC_NAME, "CFStructure")
	(MQCA_CLUSTER_DATE, "ClusterDate")
	(MQCA_CLUSTER_TIME, "ClusterTime")
	(MQIA_CLUSTER_Q_TYPE, "ClusterQType", DisplayMapInitializer
		(MQCQT_LOCAL_Q, "Local")
		(MQCQT_ALIAS_Q, "Alias")
		(MQCQT_REMOTE_Q, "Remote")
		(MQCQT_Q_MGR_ALIAS, "Qmgr Alias")
	)
	(MQIA_CLWL_Q_PRIORITY, "CLWLQueuePriority")
	(MQIA_CLWL_Q_RANK, "CLWLQueueRank")
	(MQIA_CLWL_USEQ, "CLWLUseQ", DisplayMapInitializer
		(MQCLWL_USEQ_AS_Q_MGR, "Qmgr")
		(MQCLWL_USEQ_ANY, "Any")
		(MQCLWL_USEQ_LOCAL, "Local")
	)
	(MQCA_CREATION_DATE, "CreationDate")
	(MQCA_CREATION_TIME, "CreationTime")
	(MQIA_CURRENT_Q_DEPTH, "CurrentQDepth")
	(MQIA_DEF_PUT_RESPONSE_TYPE, "DefaultPutResponse", DisplayMapInitializer
		(MQPRT_SYNC_RESPONSE, "Sync")
		(MQPRT_ASYNC_RESPONSE, "Async")
	)
	(MQIA_DEF_BIND, "DefBind", DisplayMapInitializer
		(MQBND_BIND_ON_OPEN, "On Open")
		(MQBND_BIND_NOT_FIXED, "Not Fixed")
	)
	#ifdef MQBND_BIND_ON_GROUP
		(MQBND_BIND_ON_GROUP, "On Group")
	#endif
	(MQIA_DEFINITION_TYPE, "DefinitionType", DisplayMapInitializer
		(MQQDT_PREDEFINED, "Predefined")
		(MQQDT_PERMANENT_DYNAMIC, "Permanent Dynamically")
		(MQQDT_SHARED_DYNAMIC, "Shared Dynamic")
		(MQQDT_TEMPORARY_DYNAMIC, "Temporary Dynamic")
	)
	(MQIA_DEF_INPUT_OPEN_OPTION, "DefInputOpenOption", DisplayMapInitializer
		(MQOO_INPUT_EXCLUSIVE, "Exclusive")
		(MQOO_INPUT_SHARED, "Shared")
	)
	(MQIA_DEF_PERSISTENCE, "DefPersistence", DisplayMapInitializer
		(MQPER_PERSISTENT, "Persistent")
		(MQPER_NOT_PERSISTENT, "Not Persistent")
	)
	(MQIA_DEF_PRIORITY, "DefPriority")
	(MQIA_DEF_READ_AHEAD, "DefReadAhead", DisplayMapInitializer
		(MQREADA_NO, "No")
		(MQREADA_YES, "Yes")
		(MQREADA_DISABLED, "Disabled")
	)
	(MQIA_DIST_LISTS, "DistLists", DisplayMapInitializer
		(MQDL_SUPPORTED, "Supported")
		(MQDL_NOT_SUPPORTED, "Not Supported")
	)
	(MQIA_HARDEN_GET_BACKOUT, "HardenGetBackout", DisplayMapInitializer
		(MQQA_BACKOUT_HARDENED, "Hardened")
		(MQQA_BACKOUT_NOT_HARDENED, "Not Hardened")
	)
	(MQIA_INDEX_TYPE, "IndexType", DisplayMapInitializer
		(MQIT_NONE, "None")
		(MQIT_MSG_ID, "Message ID")
		(MQIT_CORREL_ID, "Correlation ID")
		(MQIT_MSG_TOKEN, "Message Token")
		(MQIT_GROUP_ID, "Group ID")
	)
	(MQIA_INHIBIT_GET, "InhibitGet", DisplayMapInitializer
		(MQQA_GET_ALLOWED, "Allowed")
		(MQQA_GET_INHIBITED, "Inhibited")
	)
	(MQIA_INHIBIT_PUT, "InhibitPut", DisplayMapInitializer
		(MQQA_PUT_ALLOWED, "Allowed")
		(MQQA_PUT_INHIBITED, "Inhibited")
	)
	(MQCA_INITIATION_Q_NAME, "InitiationQName")
	(MQIA_MAX_MSG_LENGTH, "MaxMsgLength")
	(MQIA_MAX_Q_DEPTH, "MaxQDepth")
	(MQIA_MSG_DELIVERY_SEQUENCE, "MsgDeliverySequence", DisplayMapInitializer
		(MQMDS_PRIORITY, "priority")
		(MQMDS_FIFO, "FIFO")
	)
	(MQIA_NPM_CLASS, "NonPersistentMessageClass", DisplayMapInitializer
		(MQNPM_CLASS_NORMAL, "Normal")
		(MQNPM_CLASS_HIGH, "High")
	)
	(MQIA_OPEN_INPUT_COUNT, "OpenInputCount")
	(MQIA_OPEN_OUTPUT_COUNT, "OpenOutputCount")
	(MQIA_PAGESET_ID, "PageSetID")
	(MQCA_PROCESS_NAME, "ProcessName")
	(MQIA_PROPERTY_CONTROL, "PropertyControl", DisplayMapInitializer
		(MQPROP_COMPATIBILITY, "Compatibility")
		(MQPROP_NONE, "None")
		(MQPROP_ALL, "All")
		(MQPROP_FORCE_MQRFH2, "Force MQRFH2")
	)
	(MQIA_Q_DEPTH_HIGH_EVENT, "QDepthHighEvent", DisplayMapInitializer
		(MQEVR_DISABLED, "Disabled")
		(MQEVR_ENABLED, "Enabled")
	)
	(MQIA_Q_DEPTH_HIGH_LIMIT, "QDepthHighLimit")
	(MQIA_Q_DEPTH_LOW_EVENT, "QDepthLowEvent", DisplayMapInitializer
		(MQEVR_DISABLED, "Disabled")
		(MQEVR_ENABLED, "Enabled")
	)
	(MQIA_Q_DEPTH_LOW_LIMIT, "QDepthLowLimit")
	(MQIA_Q_DEPTH_MAX_EVENT, "QDepthMaxEvent", DisplayMapInitializer
		(MQEVR_DISABLED, "Disabled")
		(MQEVR_ENABLED, "Enabled")
	)
	(MQCA_Q_DESC, "QDesc")
	(MQCA_Q_MGR_IDENTIFIER, "QMgrIdentifier")
	(MQCA_Q_NAME, "QName")
	(MQIA_Q_SERVICE_INTERVAL, "QServiceInterval")
	(MQIA_Q_SERVICE_INTERVAL_EVENT, "QServiceIntervalEvent", DisplayMapInitializer
		(MQQSIE_HIGH, "High")
		(MQQSIE_OK, "Ok")
		(MQQSIE_NONE, "None")
	)
	(MQIA_Q_TYPE, "QType", DisplayMapInitializer
		(MQQT_ALL, "All")
		(MQQT_LOCAL, "Local")
		(MQQT_ALIAS, "Alias")
		(MQQT_CLUSTER, "Cluster")
		(MQQT_MODEL, "Model")
		(MQQT_REMOTE, "Remote")
	)
	(MQIA_ACCOUNTING_Q, "QueueAccounting", DisplayMapInitializer
		(MQMON_Q_MGR, "Qmgr")
		(MQMON_OFF, "Off")
		(MQMON_ON, "On")
	)
	(MQIA_MONITORING_Q, "QueueMonitoring", DisplayMapInitializer
		(MQMON_Q_MGR, "Qmgr")
		(MQMON_OFF, "Off")
		(MQMON_LOW, "Low")
		(MQMON_MEDIUM, "Medium")
		(MQMON_HIGH, "High")
	)
	(MQIA_STATISTICS_Q, "QueueStatistics", DisplayMapInitializer
		(MQMON_Q_MGR, "Qmgr")
		(MQMON_OFF, "Off")
		(MQMON_ON, "On")
	)
	(MQCA_REMOTE_Q_MGR_NAME, "RemoteQmgrName")
	(MQCA_REMOTE_Q_NAME, "RemoteQName")
	(MQIA_RETENTION_INTERVAL, "RetentionInterval")
	(MQIA_SCOPE, "Scope", DisplayMapInitializer
		(MQSCO_Q_MGR, "Qmgr")
		(MQSCO_CELL, "Cell")
	)
	(MQIA_SHAREABILITY, "Shareability", DisplayMapInitializer
		(MQQA_SHAREABLE, "Shareable")
		(MQQA_NOT_SHAREABLE, "Not Shareable")
	)
	(MQCA_STORAGE_CLASS, "StorageClass")
	(MQCA_TPIPE_NAME, "TpipeNames")
	(MQIA_TRIGGER_CONTROL, "TriggerControl", DisplayMapInitializer
		(MQTC_OFF, "Off")
		(MQTC_ON, "On")
	)
	(MQCA_TRIGGER_DATA, "TriggerData")
	(MQIA_TRIGGER_DEPTH, "TriggerDepth")
	(MQIA_TRIGGER_MSG_PRIORITY, "TriggerMsgPriority")
	(MQIA_TRIGGER_TYPE, "TriggerType", DisplayMapInitializer
		(MQTT_NONE, "None")
		(MQTT_FIRST, "First")
		(MQTT_EVERY, "Every")
		(MQTT_DEPTH, "Depth")
	)
	(MQIA_USAGE, "Usage", DisplayMapInitializer
		(MQUS_NORMAL, "Normal")
		(MQUS_TRANSMISSION, "Transmission")
	)
	(MQCA_XMIT_Q_NAME, "XmitQName")

	// Channel properties

	(MQIACH_BATCH_HB, "BatchHeartBeat")
	(MQIACH_BATCH_INTERVAL, "BatchInterval")
#ifdef MQIACH_BATCH_DATA_LIMIT
	(MQIACH_BATCH_DATA_LIMIT, "BatchDataLimit")
#endif
	(MQIACH_BATCH_SIZE, "BatchSize")
	(MQCACH_DESC, "ChannelDesc")
	(MQIA_MONITORING_CHANNEL, "ChannelMonitoring", DisplayMapInitializer
		(MQMON_OFF, "Off")
		(MQMON_Q_MGR, "Qmgr")
		(MQMON_LOW, "Low")
		(MQMON_MEDIUM, "Medium")
		(MQMON_HIGH, "High")
	)
	(MQCACH_CHANNEL_NAME, "ChannelName")
	(MQCACH_CHANNEL_START_DATE, "ChannelStartDate")
	(MQCACH_CHANNEL_START_TIME, "ChannelStartTime")
	(MQIA_STATISTICS_CHANNEL, "ChannelStatistics", DisplayMapInitializer
		(MQMON_OFF, "Off")
		(MQMON_Q_MGR, "Qmgr")
		(MQMON_LOW, "Low")
		(MQMON_MEDIUM, "Medium")
		(MQMON_HIGH, "High")
	)
	(MQIACH_CHANNEL_TYPE, "ChannelType", DisplayMapInitializer
		(MQCHT_ALL, "All")
		(MQCHT_SENDER, "Sender")
		(MQCHT_SERVER, "Server")
		(MQCHT_RECEIVER, "Receiver")
		(MQCHT_REQUESTER, "Requester")
		(MQCHT_SVRCONN, "Server-connection")
		(MQCHT_CLNTCONN, "Client-connection")
		(MQCHT_CLUSRCVR, "Cluster-receiver")
		(MQCHT_CLUSSDR, "Cluster-sender")
#ifdef MQCHT_MQTT
		(MQCHT_MQTT, "Telemetry")
#endif
	)
	(MQIACH_CLIENT_CHANNEL_WEIGHT, "ClientChannelWeight")
#ifdef MQCACH_CLIENT_ID
	(MQCACH_CLIENT_ID, "ClientIdentifier")
#endif
	(MQIACH_CLWL_CHANNEL_PRIORITY, "CLWLChannelPriority")
	(MQIACH_CLWL_CHANNEL_RANK, "CLWLChannelRank")
	(MQIACH_CLWL_CHANNEL_WEIGHT, "CLWLChannelWeight")
	(MQIACH_CONNECTION_AFFINITY, "ConnectionAffinity", DisplayMapInitializer
		(MQCAFTY_PREFERRED, "Preferred")
		(MQCAFTY_NONE, "None")
	)
	(MQCACH_CONNECTION_NAME, "ConnectionName")
	(MQIACH_DATA_CONVERSION, "DataConversion", DisplayMapInitializer
		(MQCDC_NO_SENDER_CONVERSION, "No")
		(MQCDC_SENDER_CONVERSION, "Sender")
	)
	(MQIACH_CHANNEL_DISP, "DefaultChannelDisposition", DisplayMapInitializer
		(MQCHLD_ALL, "All")
		(MQCHLD_PRIVATE, "Private")
		(MQCHLD_FIXSHARED, "Fix Shared")
		(MQCHLD_SHARED, "Shared")
	)
	(MQIACH_DISC_INTERVAL, "DiscInterval")
#ifdef MQIACH_DEF_RECONNECT
	(MQIACH_DEF_RECONNECT, "DefReconnect", DisplayMapInitializer
		(MQRCN_NO, "No")
		(MQRCN_YES, "Yes")
		(MQRCN_Q_MGR, "Qmgr")
		(MQRCN_DISABLED, "Disabled")
	)
#endif
	(MQIACH_HDR_COMPRESSION, "HeaderCompression", DisplayMapInitializer
		(MQCOMPRESS_NONE, "None")
		(MQCOMPRESS_SYSTEM, "System")
		(MQCOMPRESS_NOT_AVAILABLE, "Not Available")
	)
	(MQIACH_HB_INTERVAL, "HeartbeatInterval")
#ifdef MQIACH_IN_DOUBT
	(MQIACH_IN_DOUBT, "InDoubt")
#endif
#ifdef MQIACH_IN_DOUBT_IN
	(MQIACH_IN_DOUBT_IN, "InDoubtInbound")
#endif
#ifdef MQIACH_IN_DOUBT_OUT
	(MQIACH_IN_DOUBT_OUT, "InDoubtOutbound")
#endif
	(MQIACH_KEEP_ALIVE_INTERVAL, "KeepAliveInterval")
	(MQCACH_LAST_MSG_DATE, "LastMsgDate")
	(MQCACH_LAST_MSG_TIME, "LastMsgTime")
	(MQCACH_LOCAL_ADDRESS, "LocalAddress")
	(MQIACH_LONG_RETRY, "LongRetryCount")
	(MQIACH_LONG_TIMER, "LongRetryInterval")
	(MQIACH_MAX_INSTANCES, "MaxInstances")
	(MQIACH_MAX_INSTS_PER_CLIENT, "MaxInstancesPerClient")
	(MQIACH_MAX_MSG_LENGTH, "MaxMsgLength")
	(MQCACH_MCA_NAME, "MCAName")
	(MQIACH_MCA_TYPE, "MCAType", DisplayMapInitializer
		(MQMCAT_PROCESS, "Process")
		(MQMCAT_THREAD, "Thread")
	)
	(MQCACH_MCA_USER_ID, "MCAUserIdentifier")
	(MQIACH_MSG_COMPRESSION, "MessageCompression", DisplayMapInitializer
		(MQCOMPRESS_NONE, "None")
		(MQCOMPRESS_RLE, "RLE")
		(MQCOMPRESS_ZLIBFAST, "Zlib Fast")
		(MQCOMPRESS_ZLIBHIGH, "Zlib High")
		(MQCOMPRESS_ANY, "Any")
	)
	(MQCACH_MODE_NAME, "ModeName")
	(MQCACH_MSG_EXIT_NAME, "MsgExit")
#ifdef MQIACH_MSGS_RECEIVED
	(MQIACH_MSGS_RECEIVED, "MsgsReceived")
#endif
	(MQIACH_MR_COUNT, "MsgRetryCount")
	(MQCACH_MR_EXIT_NAME, "MsgRetryExit")
	(MQIACH_MR_INTERVAL, "MsgRetryInterval")
	(MQCACH_MR_EXIT_USER_DATA, "MsgRetryUserData")
#ifdef MQIACH_MSGS
	(MQIACH_MSGS, "Msgs")
#endif
#ifdef MQIACH_MSGS_SENT
	(MQIACH_MSGS_SENT, "MsgsSent")
#endif
	(MQCACH_MSG_EXIT_USER_DATA, "MsgUserData")
	(MQIACH_NETWORK_PRIORITY, "NetworkPriority")
	(MQIACH_NPM_SPEED, "NonPersistentMsgSpeed", DisplayMapInitializer
		(MQNPMS_FAST, "Fast")
		(MQNPMS_NORMAL, "Normal")
	)
	(MQCACH_PASSWORD, "Password")
	(MQIA_PROPERTY_CONTROL, "PropertyControl", DisplayMapInitializer
		(MQPROP_COMPATIBILITY, "Compatibility")
		(MQPROP_NONE, "None")
		(MQPROP_ALL, "All")
	)
	(MQIACH_PUT_AUTHORITY, "PutAuthority", DisplayMapInitializer
		(MQPA_DEFAULT, "Default")
		(MQPA_CONTEXT, "Context")
	)
	(MQCACH_RCV_EXIT_NAME, "ReceiveExit")
	(MQCACH_RCV_EXIT_USER_DATA, "ReceiveUserData")
	(MQCACH_SEC_EXIT_NAME, "SecurityExit")
	(MQCACH_SEC_EXIT_USER_DATA, "SecurityUserData")
	(MQCACH_SEND_EXIT_NAME, "SendExit")
	(MQCACH_SEND_EXIT_USER_DATA, "SendUserData")
	(MQIACH_SEQUENCE_NUMBER_WRAP, "SeqNumberWrap")
	(MQIACH_SHARING_CONVERSATIONS, "SharingConversations")
	(MQIACH_SHORT_RETRY, "ShortRetryCount")
	(MQIACH_SHORT_TIMER, "ShortRetryInterval")
	(MQCACH_SSL_CIPHER_SPEC, "SSLCipherSpec")
	(MQIACH_SSL_CLIENT_AUTH, "SSLClientAuth", DisplayMapInitializer
		(MQSCA_REQUIRED, "Required")
		(MQSCA_OPTIONAL, "Optional")
	)
	(MQCACH_SSL_PEER_NAME, "SSLPeerName")
	(MQCACH_TP_NAME, "TpName")
	(MQIACH_XMIT_PROTOCOL_TYPE, "TransportType", DisplayMapInitializer
		(MQXPT_LU62, "LU62")
		(MQXPT_TCP, "TCP")
		(MQXPT_NETBIOS, "NetBIOS")
		(MQXPT_SPX, "SPX")
		(MQXPT_DECNET, "DECnet")
	)
#ifdef MQIA_USE_DEAD_LETTER_Q
	(MQIA_USE_DEAD_LETTER_Q, "UseDLQ", DisplayMapInitializer
		(MQUSEDLQ_NO, "No")
		(MQUSEDLQ_YES, "Yes")
	)
#endif
	(MQCACH_USER_ID, "UserIdentifier")
	(MQCACH_XMIT_Q_NAME, "XmitQName")

	// Channel Status Properties
	(MQIACH_BATCHES, "Batches")
	(MQIACH_BATCH_SIZE_INDICATOR, "BatchSizeIndicator")
	(MQIACH_BUFFERS_RCVD, "BuffersReceived")
	(MQIACH_BUFFERS_SENT, "BuffersSent")
	(MQIACH_BYTES_RECEIVED, "BytesReceived")
	(MQIACH_BYTES_SENT, "BytesSent")
	(MQIACH_CHANNEL_INSTANCE_TYPE, "ChannelInstanceType", DisplayMapInitializer
		(MQOT_CURRENT_CHANNEL, "Current")
		(MQOT_SAVED_CHANNEL, "Saved")
		(MQOT_SHORT_CHANNEL, "Short")
	)
	(MQIACH_CHANNEL_STATUS, "ChannelStatus", DisplayMapInitializer
		(MQCHS_BINDING, "Binding")
		(MQCHS_STARTING, "Starting")
		(MQCHS_RUNNING, "Running")
		(MQCHS_PAUSED, "Paused")
		(MQCHS_STOPPING, "Stopping")
		(MQCHS_STOPPED, "Stopped")
		(MQCHS_RETRYING, "Retrying")
		(MQCHS_REQUESTING, "Requesting")
		(MQCHS_INITIALIZING, "Initializing")
		(MQCHS_INACTIVE, "Inactive")
	)
	(MQIACH_COMPRESSION_RATE, "CompressionRate")
	(MQIACH_COMPRESSION_TIME, "CompressionTime")
	(MQCACH_CURRENT_LUWID, "CurrentLUWID")
	(MQIACH_CURRENT_MSGS, "CurrentMsgs")
	(MQIACH_CURRENT_SEQ_NUMBER, "CurrentSequenceNumber")
	(MQIACH_CURRENT_SHARING_CONVS, "CurrentSharingConverstations")
	(MQIACH_EXIT_TIME_INDICATOR, "ExitTime")
	(MQIACH_INDOUBT_STATUS, "InDoubtStatus", DisplayMapInitializer
		(MQCHIDS_NOT_INDOUBT, "Not Indoubt")
		(MQCHIDS_INDOUBT, "Indoubt")
	)
	(MQCACH_LAST_LUWID, "LastLUWID")
	(MQIACH_LAST_SEQ_NUMBER, "LastSequenceNumber")
	(MQIACH_LONG_RETRIES_LEFT, "LongRetriesLeft")
	(MQIACH_MAX_SHARING_CONVS, "MaxSharingConversations")
	(MQCACH_MCA_JOB_NAME, "MCAJobName")
	(MQIACH_MCA_STATUS, "MCAStatus", DisplayMapInitializer
		(MQMCAS_STOPPED, "Stopped")
		(MQMCAS_RUNNING, "Running")
	)
	(MQIACH_XMITQ_MSGS_AVAILABLE, "MsgsAvailable")
	(MQIACH_NETWORK_TIME_INDICATOR, "NetTime")
	(MQCACH_REMOTE_APPL_TAG, "RemoteApplTag")
#ifdef MQCACH_REMOTE_PRODUCT
	(MQCACH_REMOTE_PRODUCT, "RemoteProduct", DisplayMapInitializer
#ifdef MQMM
		(MQMM, "Queue Manager (non z/OS Platform")
#endif
#ifdef MQMV
		(MQMV, "Queue Manager on z/OS")
#endif
#ifdef MQCC
		(MQCC, "WebSphere MQ C client")
#endif
#ifdef MQNM
		(MQNM, "WebSphere MQ .NET fully managed client")
#endif
#ifdef MQJB
		(MQJB, "WebSphere MQ Classes for JAVA")
#endif
#ifdef MQJN
		(MQJN, "WebSphere MQ Classes for JMS (normal mode)")
#endif
#ifdef MQJM
		(MQJM, "WebSphere MQ Classes for JMS (migration mode)")
#endif
#ifdef MQJU
		(MQJU, "Common Java interface to the MQI")
#endif
#ifdef MQXC
		(MQXC, "XMS client C/C++ (normal mode)")
#endif
#ifdef MQXD
		(MQXD, "XMS client C/C++ (migration mode)")
#endif
#ifdef MQXN
		(MQXN, "XMS client .NET (normal mode)")
#endif
#ifdef MQXM
		(MQXM, "XMS client .NET (migration mode)")
#endif
		(0, "")
	)
#endif
#ifdef MQCACH_REMOTE_VERSION
	(MQCACH_REMOTE_VERSION, "RemoteVersion")
#endif
	(MQCACH_SSL_CERT_ISSUER_NAME, "SSLCertRemoteIssuerName")
	(MQCACH_SSL_CERT_USER_ID, "SSLCertUserId")
	(MQCACH_SSL_KEY_RESET_DATE, "SSLKeyResetDate")
	(MQIACH_SSL_KEY_RESETS, "SSLKeyResets")
	(MQCACH_SSL_KEY_RESET_TIME, "SSLKeyResetTime")
	(MQCACH_SSL_SHORT_PEER_NAME, "SSLShortPeerName")
	(MQIACH_STOP_REQUESTED, "StopRequested", DisplayMapInitializer
		(MQCHSR_STOP_NOT_REQUESTED, "Stop Not Requested")
		(MQCHSR_STOP_REQUESTED, "Stop Requested")
	)
	(MQIACH_CHANNEL_SUBSTATE, "SubState", DisplayMapInitializer
#ifdef MQCHSSTATE_CHADEXIT
		(MQCHSSTATE_CHADEXIT, "Running Channel Auto-definition Exit")
#endif
		(MQCHSSTATE_COMPRESSING, "Compressing")
		(MQCHSSTATE_END_OF_BATCH, "End Of Batch")
#ifdef MQCHSSTATE_HANDSHAKING
		(MQCHSSTATE_HANDSHAKING, "SSL Handshaking")
#endif
		(MQCHSSTATE_HEARTBEATING, "Heartbeating")
		(MQCHSSTATE_IN_MQGET, "MQQET")
		(MQCHSSTATE_IN_MQI_CALL, "MQ API Call")
		(MQCHSSTATE_IN_MQPUT, "MQPUT")
#ifdef MQCHSSTATE_MREXIT
		(MQCHSSTATE_MREXIT, "Retry Exit")
#endif
		(MQCHSSTATE_IN_MSGEXIT, "Message Exit")
		(MQCHSSTATE_NAME_SERVER, "Name Server")
		(MQCHSSTATE_NET_CONNECTING, "Network Connect")
		(MQCHSSTATE_OTHER, "Other")
		(MQCHSSTATE_IN_RCVEXIT, "Receive Exit")
		(MQCHSSTATE_RECEIVING, "Receiving")
		(MQCHSSTATE_RESYNCHING, "Resynching")
		(MQCHSSTATE_IN_SCYEXIT, "Security Exit")
		(MQCHSSTATE_IN_SENDEXIT, "Send Exit")
		(MQCHSSTATE_SENDING, "Sending")
		(MQCHSSTATE_SERIALIZING, "Serializing")
	)
	(MQIACH_XMITQ_TIME_INDICATOR, "XQTime")
;

MQMapper::MQMapper(CommandServer& commandServer) : _commandServer(commandServer)
{
}

MQMapper::~MQMapper()
{
}

void MQMapper::mapToJSON(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json)
{
	std::vector<MQLONG> parameters = pcf->getParameters();
	for(std::vector<MQLONG>::iterator it = parameters.begin(); it != parameters.end(); ++it)
	{
		std::string name = _dictionary.getName(*it);
		if ( ! name.empty() )
		{
			Poco::JSON::Object::Ptr field = new Poco::JSON::Object();
			json->set(name, field);

			field->set("id", *it);

			if ( pcf->isNumber(*it) )
			{
				MQLONG value = pcf->getParameterNum(*it);
				field->set("value", value);

				if ( _dictionary.hasDisplayMap(*it) )
				{
					std::string displayValue = _dictionary.getDisplayValue(*it, value);
					field->set("display", displayValue.empty() 
						? Poco::format("Unknown value %ld for %ld", value, *it) : displayValue);
				}
			}
			else if ( pcf->isString(*it) )
			{
				field->set("value", pcf->getParameterString(*it));
			}
			else
			{
				//TODO:
			}
		}
	}
}

void MQMapper::mapNumberToJSON(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int parameter)
{
  if ( pcf->hasParameter(parameter) )
  {
    json->set(name, pcf->getParameterNum(parameter));
  }
}


void MQMapper::mapStringToJSON(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int parameter)
{
  if (    pcf->hasParameter(parameter) )
  {
    json->set(name, pcf->getParameterString(parameter));
  }
}


void MQMapper::mapDateToJSON(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int dateParameter, int timeParameter)
{
  if (    pcf->hasParameter(dateParameter) 
       && pcf->hasParameter(timeParameter) )
  {
    Poco::DateTime dateTime = pcf->getParameterDate(dateParameter, timeParameter);
    json->set(name, Poco::DateTimeFormatter::format(dateTime, "%d-%m-%Y %H:%M:%S"));
  }
}


}} //  Namespace MQ::Web
