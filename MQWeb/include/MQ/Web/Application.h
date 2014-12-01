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

#ifndef _MQWeb_Application_H
#define _MQWeb_Application_H

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include "Poco/JSON/TemplateCache.h"

#include "Poco/TaskManager.h"

#include "MQ/Web/QueueManagerPoolCache.h"

class MQWebApplication : public Poco::Util::ServerApplication
	/// The MQWeb server application
{
public:

	MQWebApplication();
		/// Constructor

	~MQWebApplication();
		/// Destructor

	Poco::TaskManager& taskManager();
		/// Returns the taskmanager

protected:

	void initialize(Application& self);
		/// Initializes the MQWeb server

	void uninitialize();
		/// Uninitializes the MQWeb server

	void defineOptions(Poco::Util::OptionSet& options);
		/// Defines all startup options

	void handleOption(const std::string& name, const std::string& value);
		/// Handle options.

	void displayHelp();
		/// Display the help screen

	int main(const std::vector<std::string>& args);
		/// The main program

private:
	bool _helpRequested;


	Poco::JSON::TemplateCache _cache;
		/// Only one TemplaceCache object is allowed. The best place to define it
		/// is here.

	MQ::Web::QueueManagerPoolCache _qmgrPoolCache;
		/// Only one QueueManagerPoolCache object is allowed. The best place to
		/// define it is here.

	Poco::ThreadPool _tmThreadPool;

	Poco::TaskManager _tm;
		/// Taskmanager for handling tasks (used by websocket for now)
};

inline Poco::TaskManager& MQWebApplication::taskManager()
{
	return _tm;
}

#endif //  _MQWeb_Application_H
