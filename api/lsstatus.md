---
layout: default
title: MQWeb &bull; ListenerStatusController
api: true
hideSidebar: true
---
ListenerStatusController
========================

The second part of the URI must be `lsstatus` to call the ListenerStatusController.
This controller can be used to get status information for listeners.

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
: `/api/lsstatus/inquire/PIGEON`

<div style="clear:both"> </div>

