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
#include <MQ/Check/Application.h>
#include <MQ/MQFunctions.h>
#include <MQ/MQSubsystem.h>
#include <MQ/PCF.h>
#include <MQ/MQException.h>

#include <Poco/PatternFormatter.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace Poco::Util;

MQCheckApplication::MQCheckApplication(): _helpRequested(false)
{
  addSubsystem(new MQ::MQSubsystem());
}

MQCheckApplication::~MQCheckApplication()
{
}

void MQCheckApplication::initialize(Application& self)
{
  loadConfiguration(); // load default configuration files, if present
  Application::initialize(self);
}

void MQCheckApplication::uninitialize()
{
  Application::uninitialize();
}

void MQCheckApplication::defineOptions(Poco::Util::OptionSet& options)
{
  Application::defineOptions(options);

  options.addOption(Option("help", "h", "display help information on command line arguments").required(false).repeatable(false));
  options.addOption(Option("channel", "c", "Server-connection channel").required(false)
                                                                       .repeatable(false)
                                                                       .binding("mqcheck.options.channel")
                                                                       .argument("channelname"));
  options.addOption(Option("qmgr", "m", "Queuemanager").required(false)
                                                       .repeatable(false)
                                                       .binding("mqcheck.options.qmgr")
                                                       .argument("name"));
  options.addOption(Option("server", "s", "Host and port").required(false)
                                                          .repeatable(false)
                                                          .binding("mqcheck.options.server")
                                                          .argument("host(port)"));
  options.addOption(Option("type", "t", "Type of check").required(false)
                                                        .repeatable(false)
                                                        .binding("mqcheck.options.type")
                                                        .argument("type"));
  options.addOption(Option("object", "o", "Name of object").required(false)
                                                           .repeatable(false)
                                                           .binding("mqcheck.options.object")
                                                           .argument("name"));
  options.addOption(Option("qdepth", "qd", "Queue Depth").required(false)
                                                           .repeatable(false)
                                                           .binding("mqcheck.options.qdepth")
                                                           .argument("number"));
}

void MQCheckApplication::handleOption(const std::string& name, const std::string& value)
{
  Application::handleOption(name, value);

  if (name == "help")
    _helpRequested = true;
}

void MQCheckApplication::displayHelp()
{
  HelpFormatter helpFormatter(options());
  helpFormatter.setCommand(commandName());
  helpFormatter.setUsage("OPTIONS");
  helpFormatter.setHeader("MQCheck - A tool for checking Websphere MQ Objects");
  helpFormatter.format(std::cout);
}

int MQCheckApplication::main(const std::vector<std::string>& args)
{
  if (_helpRequested)
  {
    displayHelp();
    return Application::EXIT_OK;
  }

  if ( config().has("mqcheck.options.server") )
  {
    config().setString("mq.mode", "client");
  }

  MQ::MQSubsystem& mqSystem = getSubsystem<MQ::MQSubsystem>();
  mqSystem.load();

  MQ::QueueManager::Ptr qmgr = new MQ::QueueManager(config().getString("mqcheck.options.qmgr", ""));
  if ( config().has("mqcheck.options.server") )
  {
    qmgr->connect(config().getString("mqcheck.options.channel", "SYSTEM.DEF.SVRCONN"),
                  config().getString("mqcheck.options.server"));
  }
  else
  {
    qmgr->connect();
  }

  Poco::SharedPtr<MQ::CommandServer> cmdServer = new MQ::CommandServer(qmgr, "SYSTEM.DEFAULT.MODEL.QUEUE");

  std::string type = config().getString("mqcheck.options.type");
  std::transform(type.begin(), type.end(), type.begin(), ::toupper);
  if ( type.compare("QUEUE") == 0 )
  {
    checkQueues(qmgr, *cmdServer);
  }
  else if (type.compare("CHSTATUS") == 0 )
  {
    checkChannelStatus(qmgr, *cmdServer);
  }

  return Application::EXIT_OK;
}

void MQCheckApplication::checkQueues(MQ::QueueManager::Ptr qmgr, MQ::CommandServer& cmdServer)
{
  MQ::PCF::Ptr inquireQ = new MQ::PCF(MQCMD_INQUIRE_Q);

  std::string queueName = config().getString("mqcheck.options.object", "*");
  inquireQ->addParameter(MQCA_Q_NAME, queueName);

  std::string queueDepthOption = config().getString("mqcheck.options.qdepth", "");
  int queueDepth = -1;
  if ( ! queueDepthOption.empty() )
  {
    std::stringstream str(queueDepthOption);
    str >> queueDepth;
    if ( ! str )
    {
      queueDepth = -1;
    }
  }
  if ( queueDepth != -1 )
  {
    inquireQ->addFilter(MQIA_CURRENT_Q_DEPTH, MQCFOP_NOT_LESS, queueDepth);
  }

  std::vector<Poco::SharedPtr<MQ::PCF> > commandResponse;
  cmdServer.sendCommand(inquireQ, commandResponse);

  if ( commandResponse.size() > 0 )
  {
    std::vector<Poco::SharedPtr<MQ::PCF> >::iterator it = commandResponse.begin();
    if ( (*it)->getCompletionCode() != MQCC_OK )
    {
      if ( (*it)->getReasonCode() == MQRC_UNKNOWN_OBJECT_NAME )
      {
        std::cout << "No queues found" << std::endl;
        return;
      }

      throw MQ::MQException(qmgr->name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
    }

    std::cout << std::setw(MQ_Q_NAME_LENGTH + 1) << std::left << "Queue" << " " << std::setw(6) << "Depth" << " In/Out" << std::endl;
    std::cout << std::setfill('-') << std::setw(MQ_Q_NAME_LENGTH + 1) << "-" << " " << std::setw(6) << "-" << " " << std::setw(6) << "-" << std::setfill(' ') << std::endl;

    for(; it != commandResponse.end(); it++)
    {
      std::string name = (*it)->getParameters().getString(MQCA_Q_NAME);
      std::cout << std::setw(MQ_Q_NAME_LENGTH + 1) << std::left << name;
      queueDepth = (*it)->getParameters().getNumber(MQIA_CURRENT_Q_DEPTH);
      std::cout << " " << std::setw(6) << std::right;
      if ( queueDepth == -1 )
      {
        std::cout << "-";
      }
      else
      {
        std::cout << queueDepth;
      }
      int in = (*it)->getParameters().getNumber(MQIA_OPEN_INPUT_COUNT);
      int out = (*it)->getParameters().getNumber(MQIA_OPEN_OUTPUT_COUNT);
      std::cout << " " << std::setw(3);
      if ( in == -1 )
      {
        std::cout << "-";
      }
      else
      {
        std::cout << in;
      }
      std::cout << "/";
      if ( out == -1 )
      {
        std::cout << "-";
      }
      else
      {
        std::cout << out;
      }
      std::cout << std::endl;
    }
    std::cout << std::endl << commandResponse.size() << " queue(s) found." << std::endl;
  }
}

void MQCheckApplication::checkChannelStatus(MQ::QueueManager::Ptr qmgr, MQ::CommandServer& cmdServer)
{
	MQ::PCF::Ptr inquireChannelStatus = new MQ::PCF(MQCMD_INQUIRE_CHANNEL_STATUS);

  std::string channelName = config().getString("mqcheck.options.object", "*");
  inquireChannelStatus->addParameter(MQCACH_CHANNEL_NAME, channelName);

  std::vector<Poco::SharedPtr<MQ::PCF> > commandResponse;
  cmdServer.sendCommand(inquireChannelStatus, commandResponse);

  if ( commandResponse.size() > 0 )
  {
    std::vector<Poco::SharedPtr<MQ::PCF> >::iterator it = commandResponse.begin();
    if ( (*it)->getCompletionCode() != MQCC_OK )
    {
      if ( (*it)->getReasonCode() == MQRCCF_CHL_STATUS_NOT_FOUND )
      {
        std::cout << "No channel status found" << std::endl;
        return;
      }
      throw MQ::MQException(qmgr->name(), "PCF", (*it)->getCompletionCode(), (*it)->getReasonCode());
    }

    std::cout << std::setw(MQ_CHANNEL_NAME_LENGTH + 1) << std::left << "Channel" << " " << std::setw(6) << "State" << std::endl;
    std::cout << std::setfill('-') << std::setw(MQ_CHANNEL_NAME_LENGTH + 1) << "-" << " " << std::setw(6) << "-"  << std::setfill(' ') << std::endl;

    for(; it != commandResponse.end(); it++)
    {
      std::string name = (*it)->getParameters().getString(MQCACH_CHANNEL_NAME);
      std::cout << std::setw(MQ_Q_NAME_LENGTH + 1) << std::left << name;
      switch((*it)->getParameters().getNumber(MQIACH_CHANNEL_STATUS))
      {
      case MQCHS_BINDING:
        std::cout << "Binding";
        break;
      case MQCHS_STARTING:
        std::cout << "Starting";
        break;
      case MQCHS_RUNNING:
        std::cout << "Running";
        break;
      case MQCHS_PAUSED:
        std::cout << "Paused";
        break;
      case MQCHS_STOPPING:
        std::cout << "Stopping";
        break;
      case MQCHS_RETRYING:
        std::cout << "Retrying";
        break;
      case MQCHS_STOPPED:
        std::cout << "Stopped";
        break;
      case MQCHS_REQUESTING:
        std::cout << "Requesting";
        break;
      case MQCHS_INITIALIZING:
        std::cout << "Initializing";
        break;
      }
      std::cout << std::endl;
    }
  }
}


POCO_APP_MAIN(MQCheckApplication)
