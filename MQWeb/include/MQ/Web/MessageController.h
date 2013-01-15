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

#ifndef _MQWeb_MessageController_h
#define _MQWeb_MessageController_h

#include <MQ/Web/MQController.h>
#include <MQ/Web/MapInitializer.h>

namespace MQ
{
namespace Web
{

class MessageController : public MQController
{
public:
	MessageController();


	virtual ~MessageController();


	void view();


	void list();


	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;


	std::string getDefaultAction() const;

private:
};

inline const Controller::ActionMap& MessageController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("list", static_cast<ActionFn>(&MessageController::list))
			("view", static_cast<ActionFn>(&MessageController::view));
	return actions;
}

inline std::string MessageController::getDefaultAction() const
{
	return "list";
}


} } // Namespace MQ::Web

#endif // _MQWeb_MessageController_h
