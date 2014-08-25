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

##inquire

Returns all status information attributes of the queuemanager with the given 
name. The data will be returned in the `status` property of the JSON object. 
When a WebSphere MQ error occurred an `error` property is set and the `status` 
property is not set.

> HTTP_NOT_IMPLEMENTED will be returned when this command is send to a
> queuemanager on a z/OS system. MQCMD_INQUIRE_Q_MGR_STATUS is not available for
> z/OS.

###URL Parameters

`/api/qmstatus/inquire/<QueueManager>`  

**QueueManager**

The name of the queuemanager. This parameter is required.
    
###Example

`/api/qmstatus/inquire/PIGEON`
