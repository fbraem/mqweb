---
layout: default
title: MQWeb &bull; Running MQWeb
doc_running: true
---

Running MQWeb
=============

Before you start MQWeb, make sure it is correctly [configured](/docs/configure.html).
MQWeb is a server application that can run as a Windows service or as a Unix
daemon.

Daemon Process
--------------

On Unix platforms, MQWeb can be optionally run as a daemon by giving the 
`--daemon` command line option. A daemon, when launched, immediately forks off a 
background process that does the actual work. After launching the background 
process, the foreground process exits. The current working directory for the 
daemon process is changed to the root directory ("/"), as it is common practice 
for daemon processes. Therefore, be careful when configuring the paths for 
logfiles. When MQWeb doesn't run in the background it probably means the 
logfiles can't be accessed. When running as a daemon, specifying the `--pidfile` 
option (e.g., `--pidfile=/var/run/mqweb.pid`) may be useful to record the 
process ID of the daemon in a file. The PID file will be removed when the daemon 
process terminates (but not, if it crashes).
 
    ./mqweb --daemon --pidfile=/var/run/mqweb.pid

Windows Service
---------------

On Windows platforms MQWeb can run as a Windows service. To register MQWeb
with the  Windows Service Control Manager (SCM). Start MQWeb from the command 
line, with the `/registerService` option specified. This causes the application 
to register itself with the SCM, and then exit. Similarly, an application 
registered as a service can be unregistered, by specifying the 
`/unregisterService` option. The file name of the application executable 
(excluding the .exe suffix) is used as the service name. Additionally, a more 
user-friendly name can be specified, using the /displayName option (e.g., 
`/displayName="MQWeb Service"`) and a service description can be added with the 
/description option. The startup mode (automatic or manual) for the service 
can be specified with the `/startup` option. Note that the working directory 
for an application running as a service is the Windows system directory. So
move the configuration file mqweb.properties to this directory.

WebSphere MQ service
--------------------

It's also possible to use a Websphere MQ service object to start MQWeb
automatically when a queuemanager is started. The following sample defines
a MQWEB service:

    DEFINE SERVICE(MQWEB) DESCR('MQWeb Daemon') CONTROL(STARTONLY) +
    SERVTYPE(SERVER) STARTCMD('/opt/mqweb/mqweb') +
    STARTARG('--daemon --qmgr PIGEON')

When the queuemanager PIGEON is started, the MQWeb daemon will also start.
When you have multiple queuemanagers, use the `--port` argument to specify
a unique port for the MQWeb HTTP listener. This sample doesn't include a way
to stop the MQWeb daemon. On Unix you can use the `--pidfile` argument and
use the pidfile for knowing which process to kill. When MQWeb is not stopped,
another run will immediately end because it will get a "Net Exception:
Address already in use".

Apache Proxy
------------

When you already use Apache for writing web pages around the MQWeb 
[api's](/docs/apis.hmtl), you can use [mod_proxy](http://httpd.apache.org/docs/current/mod/mod_proxy.html) 
to hide the MQWeb HTTP server from your client. This can be usefull when you
use AngularJS or any other AJAX solution. The client will never know you are
running MQWeb and you can restrict the access to MQWeb to the Apache server.

    ProxyPass /mqweb http://127.0.0.1:8081/api
    ProxyPassReverse /mqweb http://127.0.0.1:8081/api

This sample will forward all URI's that start with /mqweb to the MQWeb daemon
while replacing `/mqweb` with `/api`.

When SELinux is activated make sure Apache is allowed to connect to the network:

    /usr/sbin/setsebool httpd_can_network_connect 1
    

