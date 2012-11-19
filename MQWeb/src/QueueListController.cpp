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
#include <Poco/Net/HTMLForm.h>
#include <Poco/DateTimeFormatter.h>

#include <MQ/MQException.h>
#include <MQ/CommandServer.h>

#include <MQ/Web/QueueListController.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ
{
namespace Web
{

QueueListController::QueueListController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : Controller(request, response)
{
}


QueueListController::~QueueListController()
{

}


void QueueListController::handle()
{
  if ( _request.getMethod().compare("POST") != 0 )
  {
    _response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    return;
  }
  Poco::Net::HTMLForm form(_request, _request.stream());
  PCF::Ptr inquireQ = _commandServer->createCommand(MQCMD_INQUIRE_Q);

  std::string queueNameField = form.get("queueName", "*");
  if ( queueNameField.empty() )
  {
    queueNameField = "*";
  }
  inquireQ->addParameter(MQCA_Q_NAME, queueNameField);

  std::string queueDepthField = form.get("queueDepth", "");

  int queueDepth;
  if ( Poco::NumberParser::tryParse(queueDepthField, queueDepth) )
  {
    inquireQ->addFilter(MQIA_CURRENT_Q_DEPTH, MQCFOP_NOT_LESS, queueDepth);
  }

  std::string queueType = form.get("queueType", "all");
  if ( queueType.compare("all") != 0 )
  {
    switch(queueType[0])
    {
      case 'l': inquireQ->addParameter(MQIA_Q_TYPE, MQQT_LOCAL); break;
      case 'a': inquireQ->addParameter(MQIA_Q_TYPE, MQQT_ALIAS); break;
      case 'c': inquireQ->addParameter(MQIA_Q_TYPE, MQQT_CLUSTER); break;
      case 'r': inquireQ->addParameter(MQIA_Q_TYPE, MQQT_REMOTE); break;
      case 'm': inquireQ->addParameter(MQIA_Q_TYPE, MQQT_MODEL); break;
      default: break;
    }
  }

  std::vector<Poco::SharedPtr<PCF> > commandResponse;
  _commandServer->sendCommand(inquireQ, commandResponse);
  if ( commandResponse.size() > 0 )
  {
    std::vector<Poco::SharedPtr<PCF> >::iterator it = commandResponse.begin();
    if ( (*it)->getCompletionCode() != MQCC_OK )
    {
      if ( (*it)->getReasonCode() != MQRCCF_NONE_FOUND && 
		   (*it)->getReasonCode() != MQRC_UNKNOWN_OBJECT_NAME )
      {
        throw MQException(_qmgr->name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
      }
    }

    bool excludeSystem = form.get("queueExcludeSystem", "0").compare("1") == 0;
    bool excludeTemp = form.get("queueExcludeTemp", "0").compare("1") == 0;

    Poco::JSON::Array::Ptr queues = new Poco::JSON::Array();
    for(; it != commandResponse.end(); it++)
    {
		if (    (*it)->getReasonCode() == MQRCCF_NONE_FOUND
             || (*it)->getReasonCode() == MQRC_UNKNOWN_OBJECT_NAME )
		{
			break;
		}

		if ( (*it)->isExtendedResponse() )
		{
			continue;
		}

      Poco::JSON::Object::Ptr queue = new Poco::JSON::Object();

      std::string name = (*it)->getParameterString(MQCA_Q_NAME);
      if (    excludeSystem
           && name.compare(0, 7, "SYSTEM.") == 0 )
      {
        continue;
      }

      if (    excludeTemp
           && (*it)->getParameterNum(MQIA_DEFINITION_TYPE) == MQQDT_TEMPORARY_DYNAMIC )
      {
        continue;
      }

      queue->set("name", name);
      int type = (*it)->getParameterNum(MQIA_Q_TYPE);
      switch(type)
      {
        case MQQT_ALIAS: queue->set("type", "Alias"); break;
        case MQQT_CLUSTER: queue->set("type", "Cluster"); break;
        case MQQT_LOCAL: queue->set("type", "Local"); break;
        case MQQT_REMOTE: queue->set("type", "Remote"); break;
        case MQQT_MODEL: queue->set("type", "Model"); break;
      }
      queue->set("description", (*it)->getParameterString(MQCA_Q_DESC));

      int value = (*it)->getParameterNum(MQIA_CURRENT_Q_DEPTH);
      if ( value != -1 )
      {
        queue->set("curdepth", value);
      }

      value = (*it)->getParameterNum(MQIA_OPEN_INPUT_COUNT);
      if ( value != -1 )
      {
        queue->set("inputprocs", value);
      }

      value = (*it)->getParameterNum(MQIA_OPEN_OUTPUT_COUNT);
      if ( value != -1 )
      {
        queue->set("outputprocs", value);
      }

      queue->set("getAllowed", (*it)->getParameterNum(MQIA_INHIBIT_GET) == MQQA_GET_ALLOWED);
      queue->set("putAllowed", (*it)->getParameterNum(MQIA_INHIBIT_PUT) == MQQA_PUT_ALLOWED);

      if (type == MQQT_ALIAS )
      {
        queue->set("baseQueue", (*it)->getParameterString(MQCA_BASE_Q_NAME));
      }

      if (    type == MQQT_LOCAL
           || type == MQQT_MODEL )
      {
        if ( (*it)->getParameterNum(MQIA_USAGE) != MQUS_NORMAL )
        {
          queue->set("usage", "Transmission Queue");
        }
      }

      if ( type == MQQT_REMOTE )
      {
        queue->set("remoteQueueManager", (*it)->getParameterString(MQCA_REMOTE_Q_MGR_NAME));
        queue->set("remoteQueue", (*it)->getParameterString(MQCA_REMOTE_Q_NAME));
        queue->set("xmitQueue", (*it)->getParameterString(MQCA_XMIT_Q_NAME));
      }

      queues->add(queue);
    }
    _data->set("queues", queues);
  }

  Poco::JSON::Template::Ptr tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path("queueList.tpl"));
  poco_assert_dbg(!tpl.isNull());

  tpl->render(_data, _response.send());
  _response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
}

} } // Namespace MQ::Web
