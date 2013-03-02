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

#include <MQ/Web/Controller.h>
#include <MQ/MQSubsystem.h>

#include <Poco/Util/Application.h>
#include <Poco/NullStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ
{
namespace Web
{


Controller::Controller() : _format("html"), _data(new Poco::JSON::Object())
{
}


Controller::~Controller()
{
}

void Controller::handlePart(const Poco::Net::MessageHeader& header, std::istream& stream)
{
	//TODO
}


void Controller::handle(const std::vector<std::string>& parameters, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	_parameters = parameters;
	_request = &request;
	_response = &response;

	if ( _parameters.size() > 0 )
	{
		_action = _parameters.front();
		_parameters.erase(_parameters.begin());
	}
	else
	{
		_action = getDefaultAction();
	}

	int pos = _action.find_last_of('.');
	if ( pos != std::string::npos )
	{
		_format = _action.substr(pos+1);
		_action = _action.substr(0, pos);
	}

	for(std::vector<std::string>::iterator it = _parameters.begin(); it != _parameters.end(); ++it)
	{
		pos = it->find_first_of(':');
		if ( pos != std::string::npos )
		{
			std::string name = it->substr(0, pos);
			std::string value = it->substr(pos+1);
			_namedParameters[name] = value;
		}
	}

	_form.load(request, request.stream(), *this);

	// Make sure everything is read, otherwise this can result
	// in Bad Request error in the next call.
	Poco::NullOutputStream nos;
	Poco::StreamCopier::copyStream(request.stream(), nos);

	beforeAction();

	if ( response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK
		|| _data->has("error") )
	{
		//TODO: return error template file or json error
	}

	const ActionMap& actions = getActions();
	ActionMap::const_iterator it = actions.find(_action);
	if ( it != actions.end() )
	{
		ActionFn action = it->second;
		(this->*action)();
	}

	afterAction();
}

void Controller::render()
{
	poco_assert_dbg(!_view.isNull());

	_view->render(_data, *_response);
}


std::string Controller::htmlize(const std::string &str)
{
	std::string::const_iterator it(str.begin());
	std::string::const_iterator end(str.end());
	std::string html;
	for (; it != end; ++it)
	{
		switch (*it)
		{
		case '<': html += "&lt;"; break;
		case '>': html += "&gt;"; break;
		case '"': html += "&quot;"; break;
		case '&': html += "&amp;"; break;
		default: html += *it; break;
		}
	}
	return html;
}

} } // Namespace MQ::Web
