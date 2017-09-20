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
#ifndef _MQ_MQSubsystem_h
#define _MQ_MQSubsystem_h

#include "Poco/Util/Subsystem.h"
#include "MQ/MQFunctions.h"

namespace MQ
{

class MQSubsystem : public Poco::Util::Subsystem
	/// A POCO Subsystem for initializing Websphere MQ
{
public:

	enum ConnectionMode
	{
		BINDING_MODE,
		CLIENT_MODE
	};

	MQSubsystem();
		/// Constructor

	virtual ~MQSubsystem();
		/// Destructor

	virtual const char* name() const;
		/// Returns the name

	virtual void initialize(Poco::Util::Application& app);
		/// Initialize the subsystem

	virtual void uninitialize();
		/// Unitialize the subsystem

	bool binding() const;
		/// Returns true when Websphere MQ is loaded in binding mode.

	bool client() const;
		/// Returns true when Websphere MQ is loaded in client mode.

	MQFunctions& functions();
		/// Returns the associated MQFunctions object

	ConnectionMode mode() const;
		/// Returns the connection mode

	void load();
		/// Loads the correct library based on the connection mode.

private:
	MQFunctions _functions;

	ConnectionMode _connectionMode;
};


inline const char * MQSubsystem::name() const
{
	return "MQ";
}


inline MQFunctions& MQSubsystem::functions()
{
	return _functions;
}


inline bool MQSubsystem::binding() const
{
	return _connectionMode == BINDING_MODE;
}


inline bool MQSubsystem::client() const
{
	return _connectionMode == CLIENT_MODE;
}


inline MQSubsystem::ConnectionMode MQSubsystem::mode() const
{
	return _connectionMode;
}

} // Namespace MQ

#endif // _MQ_MQSubsystem_h
