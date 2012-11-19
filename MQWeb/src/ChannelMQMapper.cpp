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
#include <MQ/Web/ChannelMQMapper.h>
#include <MQ/MQException.h>

namespace MQ {
namespace Web {

typedef std::map<std::string, MQLONG> ChannelTypeFilterMap;

ChannelTypeFilterMap initializeTypeFilterMap()
{
  ChannelTypeFilterMap map;

  map["all"] = MQCHT_ALL;
  map["sender"] = MQCHT_SENDER;
  map["server"] = MQCHT_SERVER;
  map["receiver"] = MQCHT_RECEIVER;
  map["requester"] = MQCHT_REQUESTER;
  map["svrconn"] = MQCHT_SVRCONN;
  map["clntconn"] = MQCHT_CLNTCONN;
  map["clusrcvr"] = MQCHT_CLUSRCVR;
  map["clussdr"] = MQCHT_CLUSSDR;
#ifdef MQCHT_MQTT
  map["mqtt"] = MQCHT_MQTT;
#endif

  return map;
}

typedef std::map<MQLONG, std::string> ChannelTypeMap;

ChannelTypeMap initializeTypeMap()
{
  ChannelTypeMap map;

  map[MQCHT_SENDER] = "Sender";
  map[MQCHT_SERVER] = "Server";
  map[MQCHT_RECEIVER] = "Receiver";
  map[MQCHT_REQUESTER] = "Requester";
  map[MQCHT_SVRCONN] = "Server-connection";
  map[MQCHT_CLNTCONN] = "Client-connection";
  map[MQCHT_CLUSRCVR] = "Cluster-receiver";
  map[MQCHT_CLUSSDR] = "Cluster-sender";
#ifdef MQCHT_MQTT
  map[MQCHT_MQTT] = "Telemetry";
#endif

  return map;
}

ChannelMQMapper::ChannelMQMapper(CommandServer& commandServer) : MQMapper(commandServer)
{
}

ChannelMQMapper::~ChannelMQMapper()
{
}


void ChannelMQMapper::change(const Poco::JSON::Object::Ptr&obj)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ChannelMQMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void ChannelMQMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr ChannelMQMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
  poco_assert_dbg(!filter.isNull());

  static ChannelTypeFilterMap channelTypes = initializeTypeFilterMap();

  Poco::JSON::Array::Ptr jsonChannels = new Poco::JSON::Array();

  PCF::Ptr inquireChl = _commandServer.createCommand(MQCMD_INQUIRE_CHANNEL);

  inquireChl->addParameter(MQCACH_CHANNEL_NAME, filter->optValue<std::string>("name", "*"));
  std::string channelType = filter->optValue<std::string>("type", "all");
  ChannelTypeFilterMap::iterator it = channelTypes.find(channelType);
  if ( it != channelTypes.end() )
  {
    inquireChl->addParameter(MQIACH_CHANNEL_TYPE, it->second);
  }

  PCF::Vector commandResponse;
  _commandServer.sendCommand(inquireChl, commandResponse);
  if ( commandResponse.size() > 0 )
  {
    PCF::Vector::iterator it = commandResponse.begin();
    if ( (*it)->getReasonCode() != MQCC_OK )
    {
      if ( (*it)->getReasonCode() != MQRC_UNKNOWN_OBJECT_NAME )
      {
        throw MQException(_commandServer.qmgr().name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
      }
    }

    bool excludeSystem = filter->optValue("excludeSystem", false);

    for(; it != commandResponse.end(); it++)
    {
      if ( (*it)->getReasonCode() == MQRC_UNKNOWN_OBJECT_NAME )
        break;

	  if ( (*it)->isExtendedResponse() ) // Skip extended response
		  continue;

      std::string channelName = (*it)->getParameterString(MQCACH_CHANNEL_NAME);
      if (    excludeSystem
           && channelName.compare(0, 7, "SYSTEM.") == 0 )
      {
        continue;
      }

      Poco::JSON::Object::Ptr jsonChannel = new Poco::JSON::Object();
      jsonChannels->add(jsonChannel);

      map(*it, jsonChannel);
    }
  }

  return jsonChannels;
}

std::string ChannelMQMapper::getType(MQLONG type)
{
  static ChannelTypeMap channelTypes = initializeTypeMap();
  return channelTypes[type];
}

void ChannelMQMapper::map(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json)
{
  mapDateToJSON(pcf, json, "alterationDate", MQCA_ALTERATION_DATE, MQCA_ALTERATION_TIME);
  mapNumberToJSON(pcf, json, "batchInterval", MQIACH_BATCH_INTERVAL);
  mapNumberToJSON(pcf, json, "batchHeartBeat", MQIACH_BATCH_HB);
#ifdef MQIACH_BATCH_DATA_LIMIT
  mapNumberToJSON(pcf, json, "batchDataLimit", MQIACH_BATCH_DATA_LIMIT);
#endif
  mapNumberToJSON(pcf, json, "batchSize", MQIACH_BATCH_SIZE);
  mapStringToJSON(pcf, json, "channelName", MQCACH_CHANNEL_NAME);

  switch(pcf->getParameterNum(MQIA_MONITORING_CHANNEL))
  {
  case MQMON_OFF: json->set("channelMonitoring", "off"); break;
  case MQMON_Q_MGR: json->set("channelMonitoring", "qmgr"); break;
  case MQMON_LOW: json->set("channelMonitoring", "low"); break;
  case MQMON_MEDIUM: json->set("channelMonitoring", "medium"); break;
  case MQMON_HIGH: json->set("channelMonitoring", "high"); break;
  }

  switch(pcf->getParameterNum(MQIA_STATISTICS_CHANNEL))
  {
  case MQMON_OFF: json->set("channelStatistics", "off"); break;
  case MQMON_Q_MGR: json->set("channelStatistics", "qmgr"); break;
  case MQMON_LOW: json->set("channelStatistics", "low"); break;
  case MQMON_MEDIUM: json->set("channelStatistics", "medium"); break;
  case MQMON_HIGH: json->set("channelStatistics", "high"); break;
  }

  Poco::JSON::Object::Ptr jsonType = new Poco::JSON::Object();
  json->set("channelType", jsonType);
  std::string type = getType(pcf->getParameterNum(MQIACH_CHANNEL_TYPE));
  jsonType->set("value", type);
  jsonType->set(type, true);

  mapStringToJSON(pcf, json, "channelDesc", MQCACH_DESC);
#ifdef MQCACH_CLIENT_ID
  mapStringToJSON(pcf, json, "clientIdentifier", MQCACH_CLIENT_ID);
#endif
  mapStringToJSON(pcf, json, "clusterName", MQCA_CLUSTER_NAME);
  mapStringToJSON(pcf, json, "clusterNamelist", MQCA_CLUSTER_NAMELIST);
  mapNumberToJSON(pcf, json, "clwlChannelPriority", MQIACH_CLWL_CHANNEL_PRIORITY);
  mapNumberToJSON(pcf, json, "clwlChannelRank", MQIACH_CLWL_CHANNEL_RANK);
  mapNumberToJSON(pcf, json, "clwlChannelWeight", MQIACH_CLWL_CHANNEL_WEIGHT);
  
  switch(pcf->getParameterNum(MQIACH_CONNECTION_AFFINITY))
  {
  case MQCAFTY_PREFERRED: json->set("connectionAffinity", "preferred"); break;
  case MQCAFTY_NONE: json->set("connectionAffinity", "none"); break;
  }

  mapStringToJSON(pcf, json, "connectionName", MQCACH_CONNECTION_NAME);

  switch(pcf->getParameterNum(MQIACH_DATA_CONVERSION))
  {
  case MQCDC_NO_SENDER_CONVERSION: json->set("dataConversion", "No"); break;
  case MQCDC_SENDER_CONVERSION: json->set("dataConversion", "Sender"); break;
  }

  switch(pcf->getParameterNum(MQIACH_CHANNEL_DISP))
  {
  case MQCHLD_PRIVATE: json->set("defaultChannelDisposition", "private"); break;
  case MQCHLD_FIXSHARED: json->set("defaultChannelDisposition", "fix shared"); break;
  case MQCHLD_SHARED: json->set("defaultChannelDisposition", "shared"); break;
  }

  mapNumberToJSON(pcf, json, "discInterval", MQIACH_DISC_INTERVAL);

#ifdef MQIACH_DEF_RECONNECT
  switch(pcf->getParameterNum(MQIACH_DEF_RECONNECT))
  {
  case MQRCN_NO: json->set("defReconnect", "no"); break;
  case MQRCN_YES: json->set("defReconnect", "yes"); break;
  case MQRCN_Q_MGR: json->set("defReconnect", "qmgr"); break;
  case MQRCN_DISABLED: json->set("defReconnect", "disabled"); break;
  }
#endif

  switch(pcf->getParameterNum(MQIACH_HDR_COMPRESSION))
  {
  case MQCOMPRESS_NONE: json->set("headerCompression", "none"); break;
  case MQCOMPRESS_SYSTEM: json->set("headerCompression", "system"); break;
  }

  mapNumberToJSON(pcf, json, "heartbeatInterval", MQIACH_HB_INTERVAL);
#ifdef MQIACH_IN_DOUBT
  mapNumberToJSON(pcf, json, "inDoubt", MQIACH_IN_DOUBT);
#endif
#ifdef MQIACH_IN_DOUBT_IN
  mapNumberToJSON(pcf, json, "inDoubtIn", MQIACH_IN_DOUBT_IN);
#endif
#ifdef MQIACH_IN_DOUBT_OUT
  mapNumberToJSON(pcf, json, "inDoubtOut", MQIACH_IN_DOUBT_OUT);
#endif
  mapNumberToJSON(pcf, json, "keepAliveInterval", MQIACH_KEEP_ALIVE_INTERVAL);

  mapDateToJSON(pcf, json, "lastMsgTime", MQCACH_LAST_MSG_DATE, MQCACH_LAST_MSG_TIME);
  mapStringToJSON(pcf, json, "localAddress", MQCACH_LOCAL_ADDRESS);
  mapNumberToJSON(pcf, json, "longRetryCount", MQIACH_LONG_RETRY);
  mapNumberToJSON(pcf, json, "longRetryInterval", MQIACH_LONG_TIMER);
  mapNumberToJSON(pcf, json, "maxInstances", MQIACH_MAX_INSTANCES);
  mapNumberToJSON(pcf, json, "maxInstancesPerClient", MQIACH_MAX_INSTS_PER_CLIENT);
  mapNumberToJSON(pcf, json, "maxMsgLength", MQIACH_MAX_MSG_LENGTH);
  mapStringToJSON(pcf, json, "mcaName", MQCACH_MCA_NAME);

  switch(pcf->getParameterNum(MQIACH_MCA_TYPE))
  {
  case MQMCAT_PROCESS: json->set("mcaType", "process"); break;
  case MQMCAT_THREAD: json->set("mcaType", "thread"); break;
  }

  mapStringToJSON(pcf, json, "mcaUserIdentifier", MQCACH_MCA_USER_ID);

  switch(pcf->getParameterNum(MQIACH_MSG_COMPRESSION))
  {
  case MQCOMPRESS_NONE: json->set("messageCompression", "none"); break;
  case MQCOMPRESS_RLE: json->set("messageCompression", "rle"); break;
  case MQCOMPRESS_ZLIBFAST: json->set("messageCompression", "zlib fast"); break;
  case MQCOMPRESS_ZLIBHIGH: json->set("messageCompression", "zlib high"); break;
  case MQCOMPRESS_ANY: json->set("messageCompression", "any"); break;
  }

  mapStringToJSON(pcf, json, "modeName", MQCACH_MODE_NAME);
  mapStringToJSON(pcf, json, "msgExit", MQCACH_MSG_EXIT_NAME);
#ifdef MQIACH_MSGS_RECEIVED
  mapNumberToJSON(pcf, json, "msgsReceived", MQIACH_MSGS_RECEIVED);
#endif
  mapNumberToJSON(pcf, json, "msgRetryCount", MQIACH_MR_COUNT);
  mapStringToJSON(pcf, json, "msgRetryExit", MQCACH_MR_EXIT_NAME);
  mapNumberToJSON(pcf, json, "msgRetryInterval", MQIACH_MR_INTERVAL);
  mapStringToJSON(pcf, json, "msgRetryUserData", MQCACH_MR_EXIT_USER_DATA);
#ifdef MQIACH_MSGS
  mapNumberToJSON(pcf, json, "msgs", MQIACH_MSGS);
#endif
#ifdef MQIACH_MSGS_SENT
  mapNumberToJSON(pcf, json, "msgsSent", MQIACH_MSGS_SENT);
#endif
  mapStringToJSON(pcf, json, "msgUserData", MQCACH_MSG_EXIT_USER_DATA);
  mapNumberToJSON(pcf, json, "networkPriority", MQIACH_NETWORK_PRIORITY);

  switch(pcf->getParameterNum(MQIACH_NPM_SPEED))
  {
  case MQNPMS_FAST: json->set("nonPersistentMsgSpeed", "fast"); break;
  case MQNPMS_NORMAL: json->set("nonPersistentMsgSpeed", "normal"); break;
  }

  mapStringToJSON(pcf, json, "password", MQCACH_PASSWORD);

  switch(pcf->getParameterNum(MQIA_PROPERTY_CONTROL))
  {
  case MQPROP_COMPATIBILITY: json->set("propertyControl", "compatibility"); break;
  case MQPROP_NONE: json->set("propertyControl", "none"); break;
  case MQPROP_ALL: json->set("propertyControl", "all"); break;
  }

  switch(pcf->getParameterNum(MQIACH_PUT_AUTHORITY))
  {
  case MQPA_DEFAULT: json->set("putAuthority", "default"); break;
  case MQPA_CONTEXT: json->set("putAuthority", "context"); break;
  }

  mapStringToJSON(pcf, json, "qmgrName", MQCA_Q_MGR_NAME);

  switch(pcf->getParameterNum(MQIA_QSG_DISP))
  {
  case MQQSGD_COPY: json->set("qsgDisposition", "copy"); break;
  case MQQSGD_GROUP: json->set("qsgDisposition", "group"); break;
  case MQQSGD_Q_MGR: json->set("qsgDisposition", "qmgr"); break;
  }

  mapStringToJSON(pcf, json, "receiveExit", MQCACH_RCV_EXIT_NAME);
  mapStringToJSON(pcf, json, "receiveUserData", MQCACH_RCV_EXIT_USER_DATA);
  mapStringToJSON(pcf, json, "securityExit", MQCACH_SEC_EXIT_NAME);
  mapStringToJSON(pcf, json, "securityUserData", MQCACH_SEC_EXIT_USER_DATA);
  mapStringToJSON(pcf, json, "sendExit", MQCACH_SEND_EXIT_NAME);
  mapStringToJSON(pcf, json, "sendUserData", MQCACH_SEND_EXIT_USER_DATA);

  mapNumberToJSON(pcf, json, "seqNumberWrap", MQIACH_SEQUENCE_NUMBER_WRAP);
  mapNumberToJSON(pcf, json, "sharingConversations", MQIACH_SHARING_CONVERSATIONS);
  mapNumberToJSON(pcf, json, "shortRetryCount", MQIACH_SHORT_RETRY);
  mapNumberToJSON(pcf, json, "shortRetryInterval", MQIACH_SHORT_TIMER);
  mapStringToJSON(pcf, json, "sslCipherSpec", MQCACH_SSL_CIPHER_SPEC);

  switch(pcf->getParameterNum(MQIACH_SSL_CLIENT_AUTH))
  {
  case MQSCA_REQUIRED: json->set("sslClientAuth", "required"); break;
  case MQSCA_OPTIONAL: json->set("sslClientAuth", "optional"); break;
  }

  mapStringToJSON(pcf, json, "sslPeerName", MQCACH_SSL_PEER_NAME);
  mapStringToJSON(pcf, json, "tpName", MQCACH_TP_NAME);

  switch(pcf->getParameterNum(MQIACH_XMIT_PROTOCOL_TYPE))
  {
  case MQXPT_LU62: json->set("transportType", "LU62"); break;
  case MQXPT_TCP: json->set("transportType", "TCP"); break;
  case MQXPT_NETBIOS: json->set("transportType", "NetBIOS"); break;
  case MQXPT_SPX: json->set("transportType", "SPX"); break;
  case MQXPT_DECNET: json->set("transportType", "DECnet"); break;
  }

  mapStringToJSON(pcf, json, "userIdentifier", MQCACH_USER_ID);
  mapStringToJSON(pcf, json, "xmitQName", MQCACH_XMIT_Q_NAME);
}

}} //  Namespace MQ::Web
