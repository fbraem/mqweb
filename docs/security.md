---
layout: default
title: MQWeb &bull; Security
security: true
---

**This page is still a work in progress ...**

Security
========
You can start MQWeb with user mqm but it is recommended to run your applications 
with a dedicated user. This page shows what permissions are needed to run the 
MQWeb daemon with a dedicated user. Ofcourse it depends on how you want to use 
MQWeb.

In the examples below we assume that the user has the name mqweb and is part of 
the mqweb group.

This page is currently only for Windows / Unix platforms.

Channel
-------
When MQWeb connects in client mode, you have to secure the channel. Create a 
channel and use the username in the MCAUSER attribute.

    DEFINE CHANNEL(CL.MQWEB) CHLTYPE(SVRCONN) MCAUSER('mqweb')

Queuemanager Security
---------------------
The user needs permissions to connect to the queuemanager. MQWeb also
needs authorisation to inquire some information from the queuemanager. 
When MQWeb is used to display attributes of the queuemanager you also need 
to add the DISPLAY authorisation. Use the following SET AUTHREC statement:


    SET AUTHREC GROUP('mqweb') OBJTYPE(QMGR) AUTHADD(CONNECT,INQ,DSP)


Command Server Queues Security
------------------------------
MQWeb puts PCF messages on the command queue. The name of the command queue
is inquired from the queuemanager. By default the name is 
SYSTEM.ADMIN.COMMAND.QUEUE. 

    SET AUTHREC PROFILE(SYSTEM.ADMIN.COMMAND.QUEUE) GROUP('mqweb') OBJTYPE(QUEUE) AUTHADD(PUT)

Reply Queue
-----------
The command server will put the reply of the PCF message on a queue.
Which queue must be used can be configured in the mqweb properties file.
When no name is configured, MQWeb will use the SYSTEM.DEFAULT.MODEL.QUEUE
to create a temporary queue. Ofcourse this will result in extra security
rules.

Temporary Queues
----------------
When temporary queues are used for returning the reply messages, apply the
following SET AUTHREC statements:

    SET AUTHREC PROFILE(SYSTEM.DEFAULT.MODEL.QUEUE) GROUP('mqweb') OBJTYPE(QUEUE) AUTHADD(PUT)

Queues
------
MQWeb needs permission to display queue attributes. Only give permissions
to queues that are allowed to be viewed with MQWeb. Queues that are not
allowed to view will return a 2035 reason code and will be skipped. The
BROWSE authority is also required when MQWeb is used to browse queues.

    SET AUTHREC PROFILE(**) GROUP('mqweb') OBJTYPE(QUEUE) AUTHADD(DSP)


The following statement shows how to allow the event action on 
SYSTEM.ADMIN.QMGR.EVENT . It adds BROWSE and INQ authorisations. INQ
is needed because the event action inquires the current depth of the queue.

    SET AUTHREC PROFILE(SYSTEM.ADMIN.QMGR.EVENT) GROUP('mqweb') OBJTYPE(QUEUE) AUTHADD(BROWSE, INQ)

Channels
--------

