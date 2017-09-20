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
#ifndef _MQWeb_MQWebController_h
#define _MQWeb_MQWebController_h

#include "MQ/Web/Controller.h"
#include "MQ/Web/MapInitializer.h"

namespace MQ {
namespace Web {

class MQWebController : public Controller
	/// Controller that shows the details of MQWeb
{
public:
	MQWebController();
		/// Constructor

	virtual ~MQWebController();
		/// Destructor

	virtual const std::map<std::string, Controller::ActionFn>& getActions() const;
		/// Returns all available actions

	void inquire();
		/// Returns MQWeb information like: version, client/binding mode, ...

	void list();
		/// Returns a list of available queuemanagers. In client mode, all queuemanagers
		/// that are configured are returned, in binding mode MQWeb will try to
		/// execute the mq utility dspmq.

private:
};


inline const Controller::ActionMap& MQWebController::getActions() const
{
	static Controller::ActionMap actions
		= MapInitializer<std::string, Controller::ActionFn>
			("inquire", static_cast<ActionFn>(&MQWebController::inquire))
			("list", static_cast<ActionFn>(&MQWebController::list))
		;
	return actions;
}


} } // Namespace MQ::Web

#endif // _MQWeb_MQWebController_h
