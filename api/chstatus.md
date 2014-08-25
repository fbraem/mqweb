---
layout: default
title: MQWeb &bull; ChannelStatusController
api: true
hideSidebar: true
---
ChannelStatusController
=======================

The second part of the URI must be `chstatus` to call the ChannelStatusController.
This controller can be used to get status information from channels.

##inquire

###URL Parameters

`/api/chstatus/inquire/<QueueManager>/<ChannelName>`

**QueueManager**

The name of the queuemanager. This parameter is required!

**ChannelName** (optional)

The name of a channel. When this parameter is used, query parameters are
ignored (except for `instanceType`).

###Query Parameters

**Name** (optional)

The name of a channel. When this query parameter is not available, a *
will be used which matches all channels.

**ChannelType** (optional)

Channel Type. Possible values are `All`, `Sender`, `Server`, `Receiver`,
`Requester`, `Server-connection`, `Client-connection`, `Cluster-receiver`
and `Cluster-sender`. Default is `All`. The value is case-sensitive.
  
**InstanceType** (optional)

Channel Instance Type. Possible values are `current`, `saved` or `short`.
Default is `current`. The value is case-sensitive.

###Example
`/api/chstatus/inquire/PIGEON`  
`api/chstatus/inquire/PIGEON/CL.MQWEB`  
`api/chstatus/inquire/PIGEON?Name=CL.*&ChannelType=Server-connection`  

Returns all status attributes of channels which matches the given name. The 
data will be returned in a JSON array as the `statuses` property of the JSON 
object. When a WebSphere MQ error occurred an `error` property is set and the 
`statuses` property is not set.
