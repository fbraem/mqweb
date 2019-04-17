/*
* Copyright 2017 - KBC Group NV - Franky Braem - The MIT license
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef _MQWeb_AuthenticationInformationController_h
#define _MQWeb_AuthenticationInformationController_h

#include "MQ/Web/MQController.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

class AuthenticationInformationController : public MQController
	/// Controller that shows the status of queues
{
public:
	AuthenticationInformationController();
		/// Constructor

	virtual ~AuthenticationInformationController();
		/// Destructor

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions

	void change();
		/// Action change

	void copy();
		/// Action copy

	void create();
		/// Action create

	void inquire();
		/// Action inquire. Inquire authentication information objects and returns all data in JSON format.

	void remove();
		/// Action delete.

private:
};


inline const Controller::ActionMap& AuthenticationInformationController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("change", static_cast<ActionFn>(&AuthenticationInformationController::change))
			("copy", static_cast<ActionFn>(&AuthenticationInformationController::copy))
			("create", static_cast<ActionFn>(&AuthenticationInformationController::create))
			("delete", static_cast<ActionFn>(&AuthenticationInformationController::remove))
			("inquire", static_cast<ActionFn>(&AuthenticationInformationController::inquire))
		;
	return actions;
}


} } // Namespace MQ::Web

#endif // _MQWeb_AuthenticationInformationController_h
