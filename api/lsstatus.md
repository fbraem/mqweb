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

##inquire

Inquires the state of the listeners. The returned JSON object will have a `mqweb` object and a`statuses` array with
all returned statuses as object. When an WebSphere MQ error occurred, there will be no `statuses` array.
Instead an `error` object is returned.

###URL Parameters

`/api/lsstatus/inquire/<QueueManager>/<ListernName>`

**QueueManager**

The name of the queuemanager. This parameter is required.

**ListenerName**

The name of the listener. When this parameter is used, query parameters are ignored.

###Query Parameters

**ListenerName**

The name of the listener. When this parameter is omitted and there is no URL parameter for
a listenername, * will be used.
    
###Example

`/api/lsstatus/inquire/PIGEON`
`/api/lsstatus/inquire/PIGEON?ListenerName=MQWEB*`

