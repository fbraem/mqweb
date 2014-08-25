---
layout: default
title: MQWeb &bull; QueueManagerController
api: true
hideSidebar: true
---
QueueManagerController
======================

The second part of the URI must be `qmgr` to call the QueueManagerController.
This controller can be used to get information from a queuemanager.

##inquire

Returns all attributes of the queuemanager with the given name. The data will
be returned in the `qmgr` property of the JSON object. When a
WebSphere MQ error occurred an `error` property is set and the `qmgr` 
property is not set.

###URL Parameters

`/api/qmgr/inquire/<QueueManager>`

**QueueManager**

The name of the queuemanager. This parameter is required.
    
###Example

`/api/qmgr/inquire/PIGEON`
