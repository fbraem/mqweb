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
: `/api/qmgr/inquire/PIGEON`

<div style="clear:both"> </div>

Returns all attributes of the queuemanager with the given name. The data will
be returned in the `qmgr` property of the JSON object. When a
WebSphere MQ error occurred an `error` property is set and the `qmgr` 
property is not set.
