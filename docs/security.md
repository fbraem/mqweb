---
layout: default
title: MQWeb &bull; Security
security: true
---

> This page is still a work in progress ... 
> If you find a problem or an error please let it know
> by entering an [issue](https://github.com/fbraem/mqweb/issues)

Security
========
You can start MQWeb with user mqm but it is recommended to run your applications 
with a dedicated user. This page shows what permissions are needed to run the 
MQWeb daemon with a dedicated user. Ofcourse it depends on how you want to use 
MQWeb.

In the examples below we assume that the user has the name mqweb and is part of 
the mqweb group.

Websphere MQ Client Security
----------------------------
When MQWeb connects in client mode, you have to secure the client channel. Set 
the MCAUSER to NOACCESS.

    DEFINE CHANNEL(CL.MQWEB) +
    CHLTYPE(SVRCONN) +
    MCAUSER(NOACCESS)

By setting MCAUSER to NOACCESS, nobody can abuse channel CL.MQWEB. 
Now you have to add a channel authentication record:

    SET CHLAUTH(CL.MQWEB) TYPE(USERMAP) +
    CLNTUSER('mqweb') +
    USERSRC(MAP) +
    MCAUSER('mqweb') +
    ACTION(ADD)

The above rule tells the queuemanager "when you see a connection from the mqweb
ID on CL.MQWEB then set MCAUSER to mqweb and allow the connection".

>Channel authentication records are available in WebSphere MQ since version 7.1.
>In prior versions, you will need to use a security exit like BlockIP2.

Queuemanager Security
---------------------
The user needs permissions to connect to the queuemanager. MQWeb also
needs authorisation to inquire some information from the queuemanager. 
When MQWeb is used to display attributes of the queuemanager you also need 
to add the DISPLAY authorisation. Use the following SET AUTHREC statement:


    SET AUTHREC +
    GROUP('mqweb') +
    OBJTYPE(QMGR) +
    AUTHADD(CONNECT,INQ,DSP)

>SET AUTHREC is available in WebSpere MQ since version 7.1. Use the *setmqaut*
>commandline tool in prior versions.


Command Server Queues Security
------------------------------
MQWeb puts PCF messages on the command queue. The name of the command queue
is inquired from the queuemanager. By default the name is 
SYSTEM.ADMIN.COMMAND.QUEUE. 

    SET AUTHREC PROFILE(SYSTEM.ADMIN.COMMAND.QUEUE) +
    GROUP('mqweb') +
    OBJTYPE(QUEUE) +
    AUTHADD(PUT)

Reply Queue
-----------
The command server will put the reply of the PCF message on a queue.
Which queue must be used can be configured using the *mq.web.reply* property
in the mqweb properties file. When no name is configured, MQWeb will use the 
SYSTEM.DEFAULT.MODEL.QUEUE to create a temporary queue.

###Temporary Queues
When temporary queues are used for returning the reply messages, apply the
following SET AUTHREC statement to the model queue:

    SET AUTHREC PROFILE(SYSTEM.DEFAULT.MODEL.QUEUE) +
    GROUP('mqweb') +
    OBJTYPE(QUEUE) +
    AUTHADD(DSP,INQ,GET)

> There is no need to set authentication records for temporary queues. WebSphere
> MQ will grant access to this queue automatically.

###Configured queue
When a reply queue is configured in the mqweb properties file, MQWeb needs
permissions to use this queue. This sample uses *MQWEB.REPLY.Q01*:

    SET AUTHREC PROFILE(MQWEB.REPLY.Q01) +
    GROUP('mqweb') +
    OBJTYPE(QUEUE) +
    AUTHADD(GET)

Queues
------
MQWeb needs permission to display queue attributes. Only give permissions
to queues that are allowed to be viewed with MQWeb. Queues that are not
allowed to view will return a 2035 reason code and will be skipped. The
BROWSE authority is also required when MQWeb is used to browse queues.

    SET AUTHREC PROFILE(**) +
    GROUP('mqweb') +
    OBJTYPE(QUEUE) +
    AUTHADD(DSP)


The following statement shows how to allow the event action on 
SYSTEM.ADMIN.QMGR.EVENT . It adds BROWSE and INQ authorisations. INQ
is needed because the event action inquires the current depth of the queue.

    SET AUTHREC PROFILE(SYSTEM.ADMIN.QMGR.EVENT) +
    GROUP('mqweb') +
    OBJTYPE(QUEUE) +
    AUTHADD(BROWSE, DSP, INQ)

Channels
--------

