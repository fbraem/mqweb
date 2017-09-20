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
#ifndef _MQWeb_Application_H
#define _MQWeb_Application_H

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include "Poco/JSON/TemplateCache.h"

#include "MQ/Web/QueueManagerPoolCache.h"

class MQWebApplication : public Poco::Util::ServerApplication
	/// The MQWeb server application
{
public:

	MQWebApplication();
		/// Constructor

	~MQWebApplication();
		/// Destructor

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

	void displayVersion();
		/// Display the version

	int main(const std::vector<std::string>& args);
		/// The main program

private:
	bool _helpRequested;

	bool _versionRequested;

	Poco::JSON::TemplateCache _cache;
		/// Only one TemplaceCache object is allowed. The best place to define it
		/// is here.

	MQ::Web::QueueManagerPoolCache _qmgrPoolCache;
		/// Only one QueueManagerPoolCache object is allowed. The best place to
		/// define it is here.
};

#endif //  _MQWeb_Application_H
