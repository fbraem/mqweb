---
layout: default
title: MQWeb &bull; Configure MQWeb
doc_configure: true
---

Configure MQWeb
===============

When MQWeb is launched it will look for a configuration file. The configuration 
file must be located in the same directory as the executable or a parent 
directory of it, and must have the same base name as the executable, with one 
of the following extensions: *.properties*, *.ini* or *.xml*. The *.properties* 
file, if it exists, is loaded first, followed by the *.ini* file and the *.xml* 
file. If the MQWeb is built in debug mode (the _DEBUG preprocessor macro is 
defined) and the base name of the appication executable ends with a 'd', a 
config file without the 'd' ending its base name is also found. For example: 
"MQWebd.exe", built in debug mode, then MQWeb will automatically find a 
configuration file named *MQWeb.properties* if it exists and if 
*MQWebd.properties* cannot be found.

> Note that the working directory for an application running as a service is 
> the Windows system directory (e.g., C:\Windows\system32).

Connection Mode
---------------

By default mqweb connects in bindings mode. This can be changed by setting the 
mq.mode property to 'client' and to configure Websphere MQ client or mqweb.

    mq.mode=client 

A connection in client mode can be done in two different ways: configuring
WebSphere MQ client or adding connection configurations in the configuration 
file:

1. Configure Websphere MQ Client

   To connect to a queuemanager use a client channel definition table and set the 
   environment variables *MQCHLLIB* and *MQCHLTAB*, or use *mqclient.ini* and set 
   *ChannelDefinitionFile* and *ChannelDefinitionDirectory*.

   > On Windows the environment variable MQCLNTCF must point to mqclient.ini. 

   When only one queuemanager is used MQSERVER can be used.

2. Configure MQWeb

   Add mq.web.qmgr properties as follows:

       mq.web.qmgr.<qmgrName>.connection=<host>(<port>)
       mq.web.qmgr.<qmgrName>.channel=<channelName>

   Where &lt;qmgrName&gt; is the name of the queuemanager, &lt;host&gt; the server where
   the queuemanager is running. &lt;port&gt; is the port of the listener and
   &lt;channelName&gt; is the name of the server connection channel. When no channel
   property is set, *SYSTEM.DEFAULT.SVRCONN* will be used as default.

Default Queuemanager
--------------------

When no name is passed in the URL, mqweb will try to connect to the default
queuemanager. In bindings mode this is done by connecting with a blank
queuemanager name. In client mode this can be configured by setting
*mq.web.defaultQmgr* property. When this property doesn't exist, mqweb will 
try to connect with "*" and hopes that a client channel definition table 
is configured.

    mq.web.defaultQmgr=PIGEON

Reply Queue
-----------

To get the replies from the command server, MQWeb needs a queue where the replies
are put. By default a temporary queue based on the model queue 
SYSTEM.DEFAULT.MODEL.QUEUE is used, but this behaviour can be changed by setting 
the mq.web.reply property or the reply property of a queuemanager.

    mq.web.reply=MQWEB.REPLY.Q01

or

    mq.web.qmgr.<qmgr>.reply=MQWEB.REPLY.Q01

When you define your own local reply queue, make sure the queue is shareable and
don't forget to apply security settings.

Logging
-------

MQWeb uses 3 loggers: mq for all MQ actions, mq.web for all daemon information and mq.web.access to log all requested URI's.
To configure these loggers look at the documentation of [Poco::Util::LoggingConfigurator](http://pocoproject.org/docs/Poco.Util.LoggingConfigurator.html).
The following example will log everything (trace level) to mqweb.log in the application directory, except for the mq.web.access logger
which will be written to the access.log file in the application directory.

    logging.loggers.root.level=trace
    logging.loggers.root.channel.class=FileChannel
    logging.loggers.root.channel.pattern=%Y-%m-%d %H:%M:%S %N %P-%I %q *** %t
    logging.loggers.root.channel.path=${application.dir}/mqweb.log
    
    logging.loggers.l1.name=mq
    
    logging.loggers.l2.name=mq.web
    
    logging.loggers.l3.name=mq.web.access
    logging.loggers.l3.channel.class=FileChannel
    logging.loggers.l3.channel.pattern=%Y-%m-%d %H:%M:%S *** %t
    logging.loggers.l3.channel.path=${application.dir}/access.log

Protecting MQWeb
----------------

MQWeb can be configured to allow or deny incoming requests. First the incoming
IP address is checked against all mq.web.allow addresses. Next when the IP
address is allowed, the IP address is checked against all configured mq.web.deny
addresses. When nothing is configured, MQWeb will allow all incoming requests.

This example allows access from IP-addresses starting with 10 or 11. But denies
all IP addresses starting with 10.192.

    mq.web.allow.ip1=10*
    mq.web.allow.ip2=11*
    mq.web.deny.ip1=10\.192*

The value of the property must be a valid regular expression.
