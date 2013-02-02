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

#ifndef _MQWeb_Controller_h
#define _MQWeb_Controller_h

#include <Poco/SharedPtr.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/PartHandler.h>
#include <Poco/Net/HTMLForm.h>

#include <Poco/JSON/Object.h>

#include <MQ/QueueManager.h>
#include <MQ/CommandServer.h>

namespace MQ {
namespace Web {


class Controller : public Poco::Net::PartHandler
{
public:

	typedef void (Controller::*ActionFn)();

	
	typedef std::map<std::string, ActionFn> ActionMap;


	Controller();


	virtual ~Controller();


	Poco::JSON::Object& data();


	Poco::Net::HTMLForm& form();


	std::string getAction() const;


	virtual void beforeAction();


	virtual void afterAction();


	virtual const ActionMap& getActions() const = 0;


	virtual std::string getDefaultAction() const = 0;


	const std::vector<std::string>& getParameters() const;


	static std::string htmlize(const std::string &str);


	bool isGet() const;


	bool isPost() const;


	void set(const std::string& name, const Poco::Dynamic::Var& var);


	Poco::Net::HTTPServerRequest& request();


	Poco::Net::HTTPServerResponse& response();


	void setResponseStatus(Poco::Net::HTTPServerResponse::HTTPStatus status);


	void render(const std::string& templateFile);


	void render();


protected:


	virtual void handlePart(const Poco::Net::MessageHeader& header, std::istream& stream);


	virtual void handle(const std::vector<std::string>& parameters, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);


private:


	Poco::Net::HTTPServerRequest* _request;


	Poco::Net::HTTPServerResponse* _response;


	Poco::Net::HTMLForm _form;


	Poco::JSON::Object::Ptr _data;


	std::string _action;


	std::vector<std::string> _parameters;


	std::map<std::string, std::string> _namedParameters;


	friend class ControllerRequestHandler;
};


inline void Controller::afterAction()
{
	//default: do nothing
}

inline void Controller::beforeAction()
{
	//default: do nothing
}

inline Poco::JSON::Object& Controller::data()
{
	poco_assert_dbg(_data);

	return *_data;
}

inline Poco::Net::HTMLForm& Controller::form()
{
	return _form;
}

inline std::string Controller::getAction() const
{
	return _action;
}


inline const std::vector<std::string>& Controller::getParameters() const
{
	return _parameters;
}


inline bool Controller::isGet() const
{
	return _request->getMethod().compare("GET") == 0;
}


inline bool Controller::isPost() const
{
	return _request->getMethod().compare("POST") == 0;
}


inline void Controller::set(const std::string& name, const Poco::Dynamic::Var& var)
{
	_data->set(name, var);
}


inline void Controller::setResponseStatus(Poco::Net::HTTPServerResponse::HTTPStatus status)
{
	_response->setStatus(status);
	_response->send();
}

inline Poco::Net::HTTPServerRequest& Controller::request()
{
	poco_assert_dbg(_request);

	return *_request;
}


inline Poco::Net::HTTPServerResponse& Controller::response()
{
	poco_assert_dbg(_response);

	return *_response;
}


}} // Namespace MQ::Web

#endif // _MQWeb_Controller_h
