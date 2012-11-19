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

#include <MQ/Web/QueueController.h>
#include <MQ/MQSubsystem.h>

#include <Poco/Util/Application.h>
#include <Poco/URI.h>

#include <Poco/JSON/TemplateCache.h>

namespace MQ
{
namespace Web
{

Controller::Controller(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
  : _request(request)
  , _response(response)
  , _data(new Poco::JSON::Object())
{
  Poco::URI uri(request.getURI());
  std::vector<std::string> paths;
  uri.getPathSegments(paths);

  MQSubsystem& mqSystem = Poco::Util::Application::instance().getSubsystem<MQSubsystem>();
  Poco::Util::LayeredConfiguration& config = Poco::Util::Application::instance().config();

  if ( paths.size() > 0 )
  {
     _qmgr = new QueueManager(paths[0]);
  }
  else
  {
    if ( mqSystem.client() )
    {
      _qmgr = new QueueManager(config.getString("mq.web.defaultQmgr", "*"));
    }
    else // In bindings mode we can connect to the default queuemanager
    {
      _qmgr = new QueueManager();
    }
  }

  if ( mqSystem.binding() )
  {
    _qmgr->connect();
  }
  else
  {
    // In client mode we check for a configuration
    // When this is not available, we hope that a channel tab file
    // is configured.
    std::string qmgrConfig = "mq.web.qmgr." + _qmgr->name();
    std::string qmgrConfigConnection = qmgrConfig + ".connection";
    std::string qmgrConfigChannel = qmgrConfig + ".channel";

    if ( config.has(qmgrConfigConnection) )
    {
      std::string connection;
      std::string channel;

      try
      {
        connection = config.getString(qmgrConfigConnection);
      }
      catch(Poco::NotFoundException nfe)
      {
        poco_error_f1(Poco::Logger::get("mq.web"), "Can't find %s property in configuration file", qmgrConfigConnection);
        //TODO: redirect to error page
      }

      try
      {
        channel = config.getString(qmgrConfigChannel);
      }
      catch(Poco::NotFoundException nfe)
      {
        poco_error_f1(Poco::Logger::get("mq.web"), "Can't find %s property in configuration file", qmgrConfigChannel);
        //TODO: redirect to error page
      }
      _qmgr->connect(channel, connection);
    }
    else // Hope that there is a channel tab file available
    {
      _qmgr->connect();
    }
  }

  Poco::JSON::Object::Ptr jsonQmgr(new Poco::JSON::Object());
  _data->set("qmgr", jsonQmgr);
  jsonQmgr->set("name", _qmgr->name());
  jsonQmgr->set("zos", _qmgr->zos());

  _commandServer = new CommandServer(_qmgr, "SYSTEM.MQEXPLORER.REPLY.MODEL");
}


Controller::~Controller()
{

}

void Controller::renderTemplate(const std::string& templateFile)
{
  Poco::JSON::Template::Ptr tpl = Poco::JSON::TemplateCache::instance()->getTemplate(Poco::Path(templateFile));
  poco_assert_dbg(!tpl.isNull());

  tpl->render(_data, _response.send());
  _response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
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
