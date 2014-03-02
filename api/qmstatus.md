---
layout: default
title: MQWeb &bull; QueueManagerStatusController
api: true
hideSidebar: true
---
QueueManagerStatusController
============================

The second part of the URI must be `qmstatus` to call the QueueManagerStatusController.
This controller can be used to get status information from a queuemanager.

##Actions

###inquire

Name
: inquire

Type
: JSON

Parameters
: + **queueManager**

    The name of the queuemanager
    
Example
: `/api/qmstatus/inquire/PIGEON`

<div style="clear:both"> </div>

Returns all status information attributes of the queuemanager with the given 
name. The data will be returned in the `status` property of the JSON object. 
When a WebSphere MQ error occurred an `error` property is set and the `status` 
property is not set.

> HTTP_NOT_IMPLEMENTED will be returned when this command is send to a
> queuemanager on a z/OS system. MQCMD_INQUIRE_Q_MGR_STATUS is not available for
> z/OS.
