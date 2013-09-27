mqweb
=====

MQWeb - Administer Websphere MQ with a browser

Building MQWeb
--------------

1. Get Poco 1.5 from http://pocoproject.org
   Build and install it
2. Get premake4 from http://industriousone.com/premake and install it
3. Unzip the mqweb archive and go to the root directory of mqweb.
4. Open premake4.lua and see if you need to change the paths for the POCO and MQ include/lib folders
5. Run premake4 with a target (vs2008, codelite, gmake, ...)
6. Build mqweb with the target you specified in step 4.
7. Create mqweb.properties in the directory that contains the mqweb executable
   and change the following properties:

```
    mq.web.static=<full path to folder where the mqweb static files are stored>
    mq.web.templates=<full path to folder where the mqweb templates are stored>
    mq.web.port=<port number>
```


   When mq.web.port is omitted, 8081 will be used.

Running MQWeb
-------------

1. By default mqweb connects in bindings mode. This can be changed by 
   setting the mq.mode property to 'client' and to configure Websphere MQ 
   client or mqweb. 
   
   a) Configure Websphere MQ Client: 
   To connect to a queuemanager use a client channel definition table and 
   set the environment variables MQCHLLIB and MQCHLTAB, or use mqclient.ini 
   and set ChannelDefinitionFile and ChannelDefinitionDirectory (Note: on 
   Windows the environment variable MQCLNTCF must point to mqclient.ini). 
   When only one queuemanager is used MQSERVER can be used.
   
   b) Configure mqweb:
   Add mq.web.qmgr properties as follows:
```
    mq.mode=client
    mq.web.qmgr.<qmgrName>.connection=<host>(<port>)
    mq.web.qmgr.<qmgrName>.channel=<channelName>
```
   Where &lt;qmgrName&gt; is the name of the queuemanager, &lt;host&gt; the server where
   the queuemanager is running. &lt;port&gt; is the port of the listener and
   &lt;channelName&gt; is the name of the server connection channel.
   
2. When no name is passed in the URL, mqweb will try to connect to the default
   queuemanager. In bindings mode this is done by connecting with a blank
   queuemanager name. In client mode this can be configured by setting
   mq.web.defaultQmgr property. When this property doesn't exist, mqweb will 
   try to connect with "*" and hopes that a client channel definition table 
   is configured.

Logging
-------

MQWeb uses 3 loggers: mq for all MQ actions, mq.web for all daemon information and mq.web.access to log all requested URI's.
To configure these loggers look at the documentation of [Poco::Util::LoggingConfigurator](http://pocoproject.org/docs/Poco.Util.LoggingConfigurator.html).
The following example will log everything (trace level) to mqweb.log in the application directory, except for the mq.web.access logger
which will be written to the access.log file in the application directory.

```
# Logger configuration
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
```
