---
layout: default
title: MQWeb &bull; QueueManagerController
features: true
hideSidebar: true
---
QueueManagerController
======================

The first part of the URI must be `qmgr` to call the QueueManagerController.
This controller can be used to get information from a queuemanager.

##Actions

###index

Name
: index

Type
: HTML

Parameters
: + **queueManager** 

    The name of the queuemanager

Example
: `/qmgr/index/PIGEON`

<div style="clear:both"> </div>

Shows a dashboard page for a queuemanager

###inquire

Name
: inquire

Type
: JSON

Parameters
: + **queueManager**

    The name of the queuemanager
    
Example
: `/qmgr/inquire/PIGEON`

<div style="clear:both"> </div>

Returns all attributes of the queuemanager with the given name. The data will
be returned in the `qmgr` property of the JSON object. When a
WebSphere MQ error occurred an `error` property is set and the `qmgr` 
property is not set.
