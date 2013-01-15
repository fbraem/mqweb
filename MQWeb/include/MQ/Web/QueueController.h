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

#ifndef _MQWeb_QueueController_h
#define _MQWeb_QueueController_h

#include <MQ/Web/MQController.h>
#include <MQ/Web/MapInitializer.h>

namespace MQ {
namespace Web {

class QueueController : public MQController
	/// Controller that shows the details of a queue
{
public:
	QueueController();
		/// Constructor

	virtual ~QueueController();
		/// Destructor

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;


	std::string getDefaultAction() const;


	void list();


	void view();

private:
};


inline const Controller::ActionMap& QueueController::getActions() const
{
	static Controller::ActionMap actions 
		= MapInitializer<std::string, Controller::ActionFn>
			("list", static_cast<ActionFn>(&QueueController::list))
			("view", static_cast<ActionFn>(&QueueController::view));
	return actions;
}

inline std::string QueueController::getDefaultAction() const
{
	return "list";
}


} } // Namespace MQ::Web

#endif // _MQWeb_QueueController_h
