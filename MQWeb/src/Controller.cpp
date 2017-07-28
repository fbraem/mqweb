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

#include "MQ/Web/Controller.h"
#include "MQ/MQSubsystem.h"

#include "Poco/Util/Application.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

#include "Poco/JSON/Parser.h"

namespace MQ
{
namespace Web
{


Controller::Controller() : _data(new Poco::JSON::Object()), _request(NULL), _response(NULL)
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
		setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Invalid URI parameters");
		return;
	}

	for(std::vector<std::string>::iterator it = _parameters.begin(); it != _parameters.end(); ++it)
	{
		size_t pos = it->find_first_of(':');
		if ( pos != std::string::npos )
		{
			std::string name = it->substr(0, pos);
			std::string value = it->substr(pos+1);
			_namedParameters[name] = value;
		}
	}

	std::string contentType = request.getContentType();
	if ( contentType == "application/json" )
	{
		Poco::JSON::Parser parser;
		try
		{
			Poco::Dynamic::Var json = parser.parse(request.stream());
			if ( ! json.isEmpty() && json.type() == typeid(Poco::JSON::Object::Ptr) )
			{
				_data->set("input", json.extract<Poco::JSON::Object::Ptr>());
			}
		}
		catch(Poco::JSON::JSONException& jsone)
		{
			// Make sure everything is read, otherwise this can result
			// in Bad Request error in the next call.
			Poco::NullOutputStream nos;
			Poco::StreamCopier::copyStream(request.stream(), nos);

			setResponseStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "JSON error occurred: " + jsone.displayText());
			return;
		}
	}
	else
	{
		_form.load(request, request.stream(), *this);
	}

	// Make sure everything is read, otherwise this can result
	// in Bad Request error in the next call.
	Poco::NullOutputStream nos;
	Poco::StreamCopier::copyStream(request.stream(), nos);

	beforeAction();

	// It's possible that already an error occurred in beforeAction.
	// So check for it and don't do anything when there was already a problem.
	if ( response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK
		&& !_data->has("error") )
	{
		const ActionMap& actions = getActions();
		ActionMap::const_iterator it = actions.find(_action);
		if ( it == actions.end() )
		{
			setResponseStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND, "Invalid action '" + _action + "' specified.");
			return;
		}

		ActionFn action = it->second;
		(this->*action)();

		afterAction();
	}
}


void Controller::render()
{
	response().setChunkedTransferEncoding(true);
	response().setContentType("application/json");
	response().set("Cache-Controle", "no-cache,no-store,must-revalidate"); // HTTP 1.1
	response().set("Pragma", "no-cache"); // HTTP 1.0
	response().set("Expires", "0"); // Proxies

	Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();
	std::string origin = config.getString("mq.web.app.cors-origin", "");
	if ( !origin.empty() )
	{
		response().set("Access-Control-Allow-Origin", origin);
	}

	std::stringstream ss;

	if ( _form.has("callback") )
	{
		ss << _form.get("callback") << '(';
	}

	data().stringify(ss);
	ss.flush();

	if ( _form.has("callback") )
	{
		ss << ");";
	}

	Poco::StreamCopier::copyStream(ss, _response->send());
}

void Controller::formElementToJSONArray(const std::string& name, Poco::JSON::Array::Ptr arr)
{
	for(Poco::Net::NameValueCollection::ConstIterator it = form().find(name);
		it != form().end() && Poco::icompare(it->first, name) == 0;
		++it)
	{
		arr->add(it->second);
	}
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
