mqweb
=====

MQWeb - Administer Websphere MQ with a browser

Building MQWeb
--------------

1. Get [Poco 1.5.2](http://pocoproject.org/releases/poco-1.5.2). 
   Poco 1.5.2 is still a development release but it is stable to run
   with MQWeb. Build Poco. You can install the libraries, but it's no
   requirement (see 4).
   
   **Remark:** Please apply this [patch](https://github.com/fbraem/poco/commit/1cb2823d2241005ecc53bbff33932916bf669f38) to correct the handle of 
   unicode characters in JSON.
   
2. Get premake4 from http://industriousone.com/premake and install it.
3. Unzip the mqweb archive and go to the root directory of mqweb.
4. Open premake4.lua and see if you need to change the paths for the POCO and 
   MQ include/lib folders.
5. Run premake4 with a target (vs2008, codelite, gmake, ...).
6. Build mqweb with the target you specified in step 5.
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

1. On Windows platforms MQWeb can run as a Windows service. To register MQWeb
   with the  Windows Service Control Manager (SCM). Start MQWeb from the command 
   line, with the /registerService option specified. This causes the application 
   to register itself with the SCM, and then exit. Similarly, an application 
   registered as a service can be unregistered, by specifying the 
   /unregisterService option. The file name of the application executable 
   (excluding the .exe suffix) is used as the service name. Additionally, a more 
   user-friendly name can be specified, using the /displayName option (e.g., 
   /displayName="MQWeb Service") and a service description can be added with the 
   /description option. The startup mode (automatic or manual) for the service 
   can be specified with the /startup option. Note that the working directory 
   for an application running as a service is the Windows system directory. So
   move the configuration file mqweb.properties to this directory.
   
   On Unix platforms, MQWeb can be optionally run as a daemon by giving the 
   —daemon command line option. A daemon, when launched, immediately forks off 
   a background process that does the actual work. After launching the 
   background process, the foreground process exits. The current working 
   directory for the daemon process is changed to the root directory ("/"), as 
   it is common practice for daemon processes. Therefore, be careful when 
   configuring the paths for logfiles. When MQWeb doesn't run in the background
   it probably means the logfiles can't be accessed. When running as a daemon, 
   specifying the —pidfile option (e.g., —pidfile=/var/run/mqweb.pid) may be 
   useful to record the process ID of the daemon in a file. The PID file will 
   be removed when the daemon process terminates (but not, if it crashes).

2. By default mqweb connects in bindings mode. This can be changed by 
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
   &lt;channelName&gt; is the name of the server connection channel. When no channel
   property is set, SYSTEM.DEFAULT.SVRCONN will be used as default.
   
3. When no name is passed in the URL, mqweb will try to connect to the default
   queuemanager. In bindings mode this is done by connecting with a blank
   queuemanager name. In client mode this can be configured by setting
   mq.web.defaultQmgr property. When this property doesn't exist, mqweb will 
   try to connect with "*" and hopes that a client channel definition table 
   is configured.
   
4. To get the replies from the command server, MQWeb needs a queue where the replies
   are put. By default a temporary queue based on the model queue SYSTEM.DEFAULT.MODEL.QUEUE
   is used, but this behaviour can be changed by setting the mq.web.reply property or the
   reply property of a queuemanager.
```
    mq.web.reply=MQWEB.REPLY.Q01
```
	  or
```
    mq.web.qmgr.<qmgr>.reply=MQWEB.REPLY.Q01
```
    When you define your own local reply queue, make sure the queue is shareable.

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

Protecting MQWeb
----------------

MQWeb can be configured to allow or deny incoming requests. First the incoming
IP address is checked against all mq.web.allow addresses. Next when the IP
address is allowed, the IP address is checked against all configured mq.web.deny
addresses. When nothing is configured, MQWeb will allow all incoming requests.

This example allows access from IP-addresses starting with 10 or 11. But denies
all IP addresses starting with 10.192.

```
  mq.web.allow.ip1=10*
  mq.web.allow.ip2=11*
  mq.web.deny.ip1=10\.192*
```
The value of the property must be a valid regular expression.

Customize MQWeb
---------------

You can change the templates which come with MQWeb to get another view, but
you can also use MQWeb as a proxy to WebSphere MQ. All controllers and actions
can return JSON which can be handled in your favourite servers-side language.
URL's to the MQWeb controllers / actions has allways the same structure:

```
  http://<host:8081>/<controller>/<action>/<param1>/<param2> ...
```

By default MQWeb will return the build-in HTML, but when you use .json as 
suffix for <action>, MQWeb will return the data as JSON. The following example
will return queue details for TEST.Q01 on queuemanager PIGEON:

```
  http://localhost:8081/queue/view.json/PIGEON/TEST.Q01
```

When the controller/action is not yet available, you can open an issue request 
on https://github.com/fbraem/mqweb or implement it yourself and create a pull
request.

Third Party Software
--------------------

To bring you a view on your queuemanager in your favorite browser, MQWeb uses 
the following software:

The mqweb process:
+ [POCO](http://pocoproject.org) is used to write the portable HTTP server in C++, the core of MQWeb.

The build-in HTML pages
+ [jQuery](http://jquery.org) is a fast, small and feature-rich JavaScript library. It eases the HTML document traversal and manipulation, JSON requests, ...
+ [jQuery UI](http://jqueryui.com) is a curated set of user interface interactions, effects, widgets, and themes built on top of the jQuery JavaScript Library.
+ [KnockoutJS](http://knockoutjs.com/) is used to render the JSON, received from the HTTP server, on the client. Rendering on the client makes it possible to write HTML pages without touching the core of MQWeb.
+ [qTip2](http://qtip2.com) is used to show tool tips on the HTML pages.
+ [jQuery Form Plugin](http://malsup.com/jquery/form) allows us to easily upgrade HTML forms to use AJAJ.
+ [Formalize](http://formalize.me) is used to teach our forms some manners ...
+ [CSS-Free-Templates](http://css-free-templates.com) has been used to select a CSS template.
