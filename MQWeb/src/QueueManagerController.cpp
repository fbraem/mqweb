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

#include <MQ/Web/QueueManagerController.h>

#include <Poco/DateTimeFormatter.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ
{
namespace Web
{


QueueManagerController::QueueManagerController() : MQController()
{
}


QueueManagerController::~QueueManagerController()
{
}


void QueueManagerController::view()
{
	PCF::Ptr inquireQmgr = commandServer()->createCommand(MQCMD_INQUIRE_Q_MGR);

	Poco::JSON::Object::Ptr jsonQmgr = data().getObject("qmgr");

	jsonQmgr->set("createDate", Poco::DateTimeFormatter::format(qmgr()->creationDate(), "%d-%m-%Y %H:%M:%S"));
	jsonQmgr->set("alterDate", Poco::DateTimeFormatter::format(qmgr()->alterationDate(), "%d-%m-%Y %H:%M:%S"));
	jsonQmgr->set("id", qmgr()->id());
	jsonQmgr->set("description", qmgr()->description());

	std::vector<Poco::SharedPtr<PCF> > commandResponse;
	commandServer()->sendCommand(inquireQmgr, commandResponse);

	if ( commandResponse.size() > 0 )
	{
		jsonQmgr->set("ccsid", commandResponse[0]->getParameterNum(MQIA_CODED_CHAR_SET_ID));
		jsonQmgr->set("deadLetterQueue", commandResponse[0]->getParameterString(MQCA_DEAD_LETTER_Q_NAME));
		jsonQmgr->set("commandQueue", commandResponse[0]->getParameterString(MQCA_COMMAND_INPUT_Q_NAME));
		jsonQmgr->set("commandLevel", commandResponse[0]->getParameterNum(MQIA_COMMAND_LEVEL));

		switch(commandResponse[0]->getParameterNum(MQIA_PLATFORM))
		{
		case MQPL_AIX: jsonQmgr->set("platform", "UNIX"); break;
		case MQPL_NSK: jsonQmgr->set("platform", "Compaq NonStop Kernel"); break;
		case MQPL_OS400: jsonQmgr->set("platform", "i5/OS"); break;
		case MQPL_VMS: jsonQmgr->set("platform", "HP OpenVMS"); break;
		case MQPL_WINDOWS_NT: jsonQmgr->set("platform", "Windows"); break;
		case MQPL_ZOS: jsonQmgr->set("platform", "z/OS"); break;
		}
	}

	render("qmgr.tpl");
}


} } // Namespace MQ::Web
