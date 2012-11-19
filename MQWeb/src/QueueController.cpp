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
#include <Poco/DateTimeFormatter.h>
#include <Poco/URI.h>

#include <MQ/MQException.h>
#include <MQ/CommandServer.h>

#include <MQ/Web/QueueController.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ
{
namespace Web
{

QueueController::QueueController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : Controller(request, response)
{
}


QueueController::~QueueController()
{

}


void QueueController::handle()
{
  Poco::URI uri(_request.getURI());
  std::vector<std::string> paths;
  uri.getPathSegments(paths);

  if ( paths.size() < 3 )
  {
    _response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    return;
  }

  std::string queueName = paths[2];

  Poco::JSON::Object::Ptr jsonQmgr = _data->getObject("qmgr");
  poco_assert_dbg(!jsonQmgr.isNull());

  jsonQmgr->set("createDate", Poco::DateTimeFormatter::format(_qmgr->creationDate(), "%d-%m-%Y %H:%M:%S"));
  jsonQmgr->set("alterDate", Poco::DateTimeFormatter::format(_qmgr->alterationDate(), "%d-%m-%Y %H:%M:%S"));
  jsonQmgr->set("id", _qmgr->id());
  jsonQmgr->set("description", _qmgr->description());

  PCF::Ptr inquireQ = _commandServer->createCommand(MQCMD_INQUIRE_Q);
  inquireQ->addParameter(MQCA_Q_NAME, queueName);

  std::vector<Poco::SharedPtr<PCF> > commandResponse;
  _commandServer->sendCommand(inquireQ, commandResponse);
  if ( commandResponse.size() > 0 )
  {
    std::vector<Poco::SharedPtr<PCF> >::iterator it = commandResponse.begin();
    if ( (*it)->getReasonCode() != MQCC_OK )
    {
      throw MQException(_qmgr->name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
    }

    Poco::JSON::Object::Ptr jsonQueue(new Poco::JSON::Object());

    jsonQueue->set("name", queueName);
    int type = (*it)->getParameterNum(MQIA_Q_TYPE);
    switch(type)
    {
      case MQQT_ALIAS: jsonQueue->set("type", "Alias"); break;
      case MQQT_CLUSTER: jsonQueue->set("type", "Cluster"); break;
      case MQQT_LOCAL: jsonQueue->set("type", "Local"); break;
      case MQQT_REMOTE: jsonQueue->set("type", "Remote"); break;
      case MQQT_MODEL: jsonQueue->set("type", "Model"); break;
    }
    jsonQueue->set("description", (*it)->getParameterString(MQCA_Q_DESC));

    switch((*it)->getParameterNum(MQIA_DEFINITION_TYPE))
    {
      case MQQDT_PREDEFINED: jsonQueue->set("definitionType", "Predefined permanent queue"); break;
      case MQQDT_PERMANENT_DYNAMIC: jsonQueue->set("definitionType", "Dynamically defined permanent queue"); break;
      case MQQDT_SHARED_DYNAMIC: jsonQueue->set("definitionType", "Dynamically defined shared queue"); break;
      case MQQDT_TEMPORARY_DYNAMIC: jsonQueue->set("definitionType", "Dynamically defined temporary queue"); break;
      default: jsonQueue->set("definitionType", ""); break;
    }

    jsonQueue->set("getAllowed", (*it)->getParameterNum(MQIA_INHIBIT_GET) == MQQA_GET_ALLOWED ? "Allowed" : "Inhibited");
    jsonQueue->set("putAllowed", (*it)->getParameterNum(MQIA_INHIBIT_PUT) == MQQA_PUT_ALLOWED ? "Allowed" : "Inhibited");

    if ( (*it)->hasParameter(MQCA_CREATION_DATE) )
    {
      Poco::DateTime creationDate = (*it)->getParameterDate(MQCA_CREATION_DATE, MQCA_CREATION_TIME);
      jsonQueue->set("createDate", Poco::DateTimeFormatter::format(creationDate, "%Y-%m-%d %H:%M:%S"));
    }
    else
    {
      jsonQueue->set("createDate", "-");
    }

    Poco::DateTime alterationDate = (*it)->getParameterDate(MQCA_ALTERATION_DATE, MQCA_ALTERATION_TIME);
    jsonQueue->set("alterDate", Poco::DateTimeFormatter::format(alterationDate, "%Y-%m-%d %H:%M:%S"));

    if ( type == MQQT_ALIAS )
    {
      jsonQueue->set("baseQueue", (*it)->getParameterString(MQCA_BASE_Q_NAME));
    }

    jsonQueue->set("persist", (*it)->getParameterNum(MQIA_DEF_PERSISTENCE) == MQPER_PERSISTENT ? "Message is persistent" : "Message is not persistent");
    jsonQueue->set("priority", (*it)->getParameterNum(MQIA_DEF_PRIORITY));
    jsonQueue->set("response", (*it)->getParameterNum(MQIA_DEF_PUT_RESPONSE_TYPE) == MQPRT_SYNC_RESPONSE ? "Synchronously" : "Asynchronously");

    if ( type == MQQT_LOCAL )
    {
      jsonQueue->set("curdepth", (*it)->getParameterNum(MQIA_CURRENT_Q_DEPTH));
      jsonQueue->set("inputprocs", (*it)->getParameterNum(MQIA_OPEN_INPUT_COUNT));
      jsonQueue->set("outputprocs", (*it)->getParameterNum(MQIA_OPEN_INPUT_COUNT));
    }

    if (    type == MQQT_LOCAL
         || type == MQQT_MODEL )
    {
      jsonQueue->set("backoutQueue", (*it)->getParameterString(MQCA_BACKOUT_REQ_Q_NAME));
      jsonQueue->set("backoutThreshold", (*it)->getParameterNum(MQIA_BACKOUT_THRESHOLD));
      jsonQueue->set("hardenBackout", (*it)->getParameterNum(MQIA_HARDEN_GET_BACKOUT) == MQQA_BACKOUT_HARDENED ? "Yes" : "No");
      jsonQueue->set("defPutRespType", (*it)->getParameterNum(MQIA_DEF_PUT_RESPONSE_TYPE) == MQPRT_SYNC_RESPONSE ? "Synchronously" : "Asynchronously");
      jsonQueue->set("inputOpenOption", (*it)->getParameterNum(MQIA_DEF_INPUT_OPEN_OPTION) == MQOO_INPUT_EXCLUSIVE ? "Exclusive" : "Shared");
      jsonQueue->set("distributionList", (*it)->getParameterNum(MQIA_DIST_LISTS) == MQDL_SUPPORTED ? "Supported" : "Not Supported");
      jsonQueue->set("initiationQueue", (*it)->getParameterString(MQCA_INITIATION_Q_NAME));
      jsonQueue->set("maxMessageLength", (*it)->getParameterNum(MQIA_MAX_MSG_LENGTH));
      jsonQueue->set("maxQueueDepth", (*it)->getParameterNum(MQIA_MAX_Q_DEPTH));
      jsonQueue->set("messageDelivery", (*it)->getParameterNum(MQIA_MSG_DELIVERY_SEQUENCE) == MQMDS_PRIORITY ? "Priority" : "FIFO");
      jsonQueue->set("process", (*it)->getParameterString(MQCA_PROCESS_NAME));
      jsonQueue->set("depthHighEvent", (*it)->getParameterNum(MQIA_Q_DEPTH_HIGH_EVENT) == MQEVR_DISABLED ? "Disabled" : "Enabled");
      jsonQueue->set("depthHighLimit", (*it)->getParameterNum(MQIA_Q_DEPTH_HIGH_LIMIT));
      jsonQueue->set("depthLowEvent", (*it)->getParameterNum(MQIA_Q_DEPTH_LOW_EVENT) == MQEVR_DISABLED ? "Disabled" : "Enabled");
      jsonQueue->set("depthLowLimit", (*it)->getParameterNum(MQIA_Q_DEPTH_LOW_LIMIT));
      jsonQueue->set("depthMaxEvent", (*it)->getParameterNum(MQIA_Q_DEPTH_MAX_EVENT) == MQEVR_DISABLED ? "Disabled" : "Enabled");
      jsonQueue->set("triggerControl", (*it)->getParameterNum(MQIA_TRIGGER_CONTROL) == MQTC_OFF ? "Off" : "On");
      jsonQueue->set("triggerData", (*it)->getParameterString(MQCA_TRIGGER_DATA));
      jsonQueue->set("triggerDepth", (*it)->getParameterNum(MQIA_TRIGGER_DEPTH));
      jsonQueue->set("triggerMessagePriority", (*it)->getParameterNum(MQIA_TRIGGER_MSG_PRIORITY));
      switch((*it)->getParameterNum(MQIA_TRIGGER_TYPE))
      {
        case MQTT_NONE: jsonQueue->set("triggerType", "None"); break;
        case MQTT_FIRST: jsonQueue->set("triggerType", "First"); break;
        case MQTT_EVERY: jsonQueue->set("triggerType", "Every"); break;
        case MQTT_DEPTH: jsonQueue->set("triggerType", "Depth"); break;
        default: break;
      }
      jsonQueue->set("usage", (*it)->getParameterNum(MQIA_USAGE) == MQUS_NORMAL ? "Normal" : "Transmission Queue");
    }
    else if ( type == MQQT_REMOTE )
    {
      jsonQueue->set("remoteQueueManager", (*it)->getParameterString(MQCA_REMOTE_Q_MGR_NAME));
      jsonQueue->set("remoteQueue", (*it)->getParameterString(MQCA_REMOTE_Q_NAME));
      jsonQueue->set("xmitQueue", (*it)->getParameterString(MQCA_XMIT_Q_NAME));
    }

    if ( (*it)->hasParameter(MQIA_DEF_READ_AHEAD) )
    {
      switch((*it)->getParameterNum(MQIA_DEF_READ_AHEAD))
      {
        case MQREADA_NO: jsonQueue->set("readAhead", "No"); break;
        case MQREADA_YES: jsonQueue->set("readAhead", "Yes"); break;
        case MQREADA_DISABLED: jsonQueue->set("readAhead", "Disabled"); break;
        default: jsonQueue->set("readAhead", "-"); break;
      }
    }
    else
    {
      jsonQueue->set("readAhead", "-");
    }

    jsonQueue->set("bind", (*it)->getParameterNum(MQIA_DEF_BIND) == MQBND_BIND_ON_OPEN ? "Binding fixed by MQOPEN call" : "Binding not fixed");

    _data->set("queue", jsonQueue);
  }

  Poco::JSON::Template::Ptr tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path("queue.tpl"));
  poco_assert_dbg(!tpl.isNull());

  tpl->render(_data, _response.send());
  _response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
}


} } // Namespace MQ::Web
