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
#include <sstream>

#include <MQ/CommandServer.h>

#include <MQ/Web/QueueManagerController.h>

#include <Poco/Util/Application.h>
#include <Poco/DateTimeFormatter.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ
{
namespace Web
{


QueueManagerController::QueueManagerController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : Controller(request, response)
{
}


QueueManagerController::~QueueManagerController()
{

}


void QueueManagerController::handle()
{
	PCF::Ptr inquireQmgr = _commandServer->createCommand(MQCMD_INQUIRE_Q_MGR);

  Poco::JSON::Object::Ptr jsonQmgr = _data->getObject("qmgr");
  poco_assert_dbg(!jsonQmgr.isNull());

  jsonQmgr->set("createDate", Poco::DateTimeFormatter::format(_qmgr->creationDate(), "%d-%m-%Y %H:%M:%S"));
  jsonQmgr->set("alterDate", Poco::DateTimeFormatter::format(_qmgr->alterationDate(), "%d-%m-%Y %H:%M:%S"));
  jsonQmgr->set("id", _qmgr->id());
  jsonQmgr->set("description", _qmgr->description());

  std::vector<Poco::SharedPtr<PCF> > commandResponse;
  _commandServer->sendCommand(inquireQmgr, commandResponse);

  if ( commandResponse.size() > 0 )
  {
    jsonQmgr->set("ccsid", commandResponse[0]->getParameterNum(MQIA_CODED_CHAR_SET_ID));
    jsonQmgr->set("deadLetterQueue", commandResponse[0]->getParameterString(MQCA_DEAD_LETTER_Q_NAME));
    jsonQmgr->set("commandQueue", commandResponse[0]->getParameterString(MQCA_COMMAND_INPUT_Q_NAME));
    jsonQmgr->set("commandLevel", commandResponse[0]->getParameterNum(MQIA_COMMAND_LEVEL));

    switch(commandResponse[0]->getParameterNum(MQIA_PLATFORM))
    {
    case MQPL_AIX:
      jsonQmgr->set("platform", "UNIX");
      break;
    case MQPL_NSK:
      jsonQmgr->set("platform", "Compaq NonStop Kernel");
      break;
    case MQPL_OS400:
      jsonQmgr->set("platform", "i5/OS");
      break;
    case MQPL_VMS:
      jsonQmgr->set("platform", "HP OpenVMS");
      break;
    case MQPL_WINDOWS_NT:
      jsonQmgr->set("platform", "Windows");
      break;
    case MQPL_ZOS:
      jsonQmgr->set("platform", "z/OS");
      break;
    }
  }

  Poco::JSON::Template::Ptr tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path("qmgr.tpl"));
  poco_assert_dbg(!tpl.isNull());

  tpl->render(_data, _response.send());
  _response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
}


} } // Namespace MQ::Web
