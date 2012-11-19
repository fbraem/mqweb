/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or ? as soon they
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
#include <MQ/Web/QueueMQMapper.h>
#include <MQ/MQException.h>

namespace MQ {
namespace Web {

typedef std::map<std::string, MQLONG> QueueTypeFilterMap;

QueueTypeFilterMap initializeQTypeFilterMap()
{
  QueueTypeFilterMap map;

  map["all"] = MQQT_ALL;
  map["local"] = MQQT_LOCAL;
  map["alias"] = MQQT_ALIAS;
  map["cluster"] = MQQT_CLUSTER;
  map["model"] = MQQT_MODEL;
  map["remote"] = MQQT_REMOTE;

  return map;
}

typedef std::map<MQLONG, std::string> QueueTypeMap;

QueueTypeMap initializeQTypeMap()
{
  QueueTypeMap map;

  map[MQQT_LOCAL] = "Local";
  map[MQQT_ALIAS] = "Alias";
  map[MQQT_CLUSTER] = "Cluster";
  map[MQQT_MODEL] = "Model";
  map[MQQT_REMOTE] = "Remote";

  return map;
}

QueueMQMapper::QueueMQMapper(CommandServer& commandServer) : MQMapper(commandServer)
{
}

QueueMQMapper::~QueueMQMapper()
{
}


void QueueMQMapper::change(const Poco::JSON::Object::Ptr&obj)
{
  poco_assert_dbg(false); // Not yet implemented
}


void QueueMQMapper::create(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


void QueueMQMapper::copy(const Poco::JSON::Object::Ptr& obj, bool replace)
{
  poco_assert_dbg(false); // Not yet implemented
}


Poco::JSON::Array::Ptr QueueMQMapper::inquire(const Poco::JSON::Object::Ptr& filter)
{
  poco_assert_dbg(!filter.isNull());

  static QueueTypeFilterMap queueTypes = initializeQTypeFilterMap();

  Poco::JSON::Array::Ptr jsonQueues = new Poco::JSON::Array();

  PCF::Ptr inquireQ = _commandServer.createCommand(MQCMD_INQUIRE_Q);
  inquireQ->addParameter(MQCA_Q_NAME, filter->optValue<std::string>("name", "*"));
  if ( filter->has("qdepth") )
  {
    inquireQ->addFilter(MQIA_CURRENT_Q_DEPTH, MQCFOP_NOT_LESS, filter->getValue<int>("qdepth"));
  }

  std::string queueType = filter->optValue<std::string>("type", "all");
  QueueTypeFilterMap::iterator it = queueTypes.find(queueType);
  if ( it != queueTypes.end() )
  {
    inquireQ->addParameter(MQIA_Q_TYPE, it->second);
  }

  PCF::Vector commandResponse;
  _commandServer.sendCommand(inquireQ, commandResponse);
  if ( commandResponse.size() > 0 )
  {
    PCF::Vector::iterator it = commandResponse.begin();
    if ( (*it)->getCompletionCode() != MQCC_OK )
    {
      if ( (*it)->getReasonCode() != MQRC_UNKNOWN_OBJECT_NAME )
      {
        throw MQException(_commandServer.qmgr().name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
      }
    }

    bool excludeSystem = filter->optValue("excludeSystem", false);
    bool excludeTemp = filter->optValue("excludeTemp", false);

    for(; it != commandResponse.end(); it++)
    {
      std::string qName = (*it)->getParameterString(MQCACH_CHANNEL_NAME);
      if (    excludeSystem
           && qName.compare(0, 7, "SYSTEM.") == 0 )
      {
        continue;
      }

      if (    excludeTemp
           && (*it)->getParameterNum(MQIA_DEFINITION_TYPE) == MQQDT_TEMPORARY_DYNAMIC )
      {
        continue;
      }

      Poco::JSON::Object::Ptr jsonQueue = new Poco::JSON::Object();
      jsonQueues->add(jsonQueue);

      map(*it, jsonQueue);
    }
  }

  return jsonQueues;
}

std::string QueueMQMapper::getType(MQLONG type)
{
  static QueueTypeMap queueTypes = initializeQTypeMap();
  return queueTypes[type];
}

void QueueMQMapper::map(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json)
{
  mapDateToJSON(pcf, json, "alterationDate", MQCA_ALTERATION_DATE, MQCA_ALTERATION_TIME);
  mapStringToJSON(pcf, json, "backoutRequeueName", MQCA_BACKOUT_REQ_Q_NAME);
  mapNumberToJSON(pcf, json, "backoutThreshold", MQIA_BACKOUT_THRESHOLD);
  mapStringToJSON(pcf, json, "baseQName", MQCA_BASE_Q_NAME);
  mapStringToJSON(pcf, json, "cfStructure", MQCA_CF_STRUC_NAME);
  
  mapDateToJSON(pcf, json, "clusterDate", MQCA_CLUSTER_DATE, MQCA_CLUSTER_TIME);
  mapStringToJSON(pcf, json, "clusterName", MQCA_CLUSTER_NAME);
  mapStringToJSON(pcf, json, "clusterNamelist", MQCA_CLUSTER_NAMELIST);
  switch(pcf->getParameterNum(MQIA_CLUSTER_Q_TYPE))
  {
  case MQCQT_LOCAL_Q: json->set("clusterQType", "Local"); break;
  case MQCQT_ALIAS_Q: json->set("clusterQType", "Alias"); break;
  case MQCQT_REMOTE_Q: json->set("clusterQType", "Remote"); break;
  case MQCQT_Q_MGR_ALIAS: json->set("clusterQType", "Qmgr Alias"); break;
  }

  mapNumberToJSON(pcf, json, "clwlQueuePriority", MQIA_CLWL_Q_PRIORITY);
  mapNumberToJSON(pcf, json, "clwlQueueRank", MQIA_CLWL_Q_RANK);
  switch(pcf->getParameterNum(MQIA_CLWL_USEQ))
  {
  case MQCLWL_USEQ_AS_Q_MGR: json->set("clwlUseQ", "Qmgr"); break;
  case MQCLWL_USEQ_ANY: json->set("clwlUseQ", "Any"); break;
  case MQCLWL_USEQ_LOCAL: json->set("clwlUseQ", "Local"); break;
  }

  mapDateToJSON(pcf, json, "creationDate", MQCA_CREATION_DATE, MQCA_CREATION_TIME);
  mapNumberToJSON(pcf, json, "currentQDepth", MQIA_CURRENT_Q_DEPTH);

  switch(pcf->getParameterNum(MQIA_DEF_PUT_RESPONSE_TYPE))
  {
  case MQPRT_SYNC_RESPONSE: json->set("defaultPutResponse", "Sync"); break;
  case MQPRT_ASYNC_RESPONSE: json->set("defaultPutResponse", "Async"); break;
  }

  switch(pcf->getParameterNum(MQIA_DEF_BIND))
  {
  case MQBND_BIND_ON_OPEN: json->set("defBind", "On Open"); break;
  case MQBND_BIND_NOT_FIXED: json->set("defBind", "Not Fixed"); break;
#ifdef MQBND_BIND_ON_GROUP
  case MQBND_BIND_ON_GROUP: json->set("defBind", "On Group"); break;
#endif
  }

  switch(pcf->getParameterNum(MQIA_DEFINITION_TYPE))
  {
    case MQQDT_PREDEFINED: json->set("definitionType", "Predefined"); break;
    case MQQDT_PERMANENT_DYNAMIC: json->set("definitionType", "Permanent Dynamically"); break;
    case MQQDT_SHARED_DYNAMIC: json->set("definitionType", "Shared Dynamic"); break;
    case MQQDT_TEMPORARY_DYNAMIC: json->set("definitionType", "Temporary Dynamic"); break;
  }

  switch(pcf->getParameterNum(MQIA_DEF_INPUT_OPEN_OPTION))
  {
  case MQOO_INPUT_EXCLUSIVE: json->set("defInputOpenOption", "Exclusive"); break;
  case MQOO_INPUT_SHARED: json->set("defInputOpenOption", "Shared"); break;
  }

  switch(pcf->getParameterNum(MQIA_DEF_PERSISTENCE))
  {
  case MQPER_PERSISTENT: json->set("defPersistence", "Persistent"); break;
  case MQPER_NOT_PERSISTENT: json->set("defPersistence", "Not Persistent"); break;
  }

  switch(pcf->getParameterNum(MQIA_DEF_READ_AHEAD))
  {
  case MQREADA_NO: json->set("defReadAhead", "No"); break;
  case MQREADA_YES: json->set("defReadAhead", "Yes"); break;
  case MQREADA_DISABLED: json->set("defReadAhead", "Disabled"); break;
  }

  switch(pcf->getParameterNum(MQIA_DIST_LISTS))
  {
  case MQDL_SUPPORTED: json->set("distLists", "Supported"); break;
  case MQDL_NOT_SUPPORTED: json->set("distLists", "Not Supported"); break;
  }

  switch(pcf->getParameterNum(MQIA_HARDEN_GET_BACKOUT))
  {
  case MQQA_BACKOUT_HARDENED: json->set("hardenGetBackout", "Hardened"); break;
  case MQQA_BACKOUT_NOT_HARDENED: json->set("hardenGetBackout", "Not Hardened"); break;
  }

  switch(pcf->getParameterNum(MQIA_INDEX_TYPE))
  {
  case MQIT_NONE: json->set("indexType", "None"); break;
  case MQIT_MSG_ID: json->set("indexType", "Message ID"); break;
  case MQIT_CORREL_ID: json->set("indexType", "Correlation ID"); break;
  case MQIT_MSG_TOKEN: json->set("indexType", "Message Token"); break;
  case MQIT_GROUP_ID: json->set("indexType", "Group ID"); break;
  }

  switch(pcf->getParameterNum(MQIA_INHIBIT_GET))
  {
  case MQQA_GET_ALLOWED: json->set("inhibitGet", "Allowed"); json->set("getAllowed", true); break;
  case MQQA_GET_INHIBITED: json->set("inhibitGet", "Inhibited"); json->set("getAllowed", false); break;
  }

  switch(pcf->getParameterNum(MQIA_INHIBIT_PUT))
  {
  case MQQA_PUT_ALLOWED: json->set("inhibitPut", "Allowed"); json->set("putAllowed", true); break;
  case MQQA_PUT_INHIBITED: json->set("inhibitPut", "Inhibited"); json->set("putAllowed", false); break;
  }

  Poco::JSON::Object::Ptr jsonType = new Poco::JSON::Object();
  json->set("queueType", jsonType);
  std::string type = getType(pcf->getParameterNum(MQIA_Q_TYPE));
  jsonType->set("value", type);
  jsonType->set(type, true);

}

}} //  Namespace MQ::Web
