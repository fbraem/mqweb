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

##Actions

###inquire

Name
: inquire

Type
: JSON

Parameters
: + **queueManager**

    The name of the queuemanager

  + **channelName** (optional)

    The name of a channel. When this parameter is used, query parameters are
    ignored (except for `instanceType`).

Query Parameters
: + **channelName** (optional)

    The name of a channel. When this query parameter is not available, a *
    will be used which matches all channels.

  + **channelType** (optional)

    Channel Type. Possible values are `All`, `Sender`, `Server`, `Receiver`,
    `Requester`, `Server-connection`, `Client-connection`, `Cluster-receiver`
    and `Cluster-sender`. Default is `All`.
  
  + **instanceType** (optional)

    Channel Instance Type. Possible values are `current`, `saved` or `short`.
    Default is `current`.

Example
: `/api/chstatus/inquire/PIGEON`  
  `api/chstatus/inquire/PIGEON/CL.MQWEB`  
  `api/chstatus/inquire/PIGEON?name=CL.*&channelType=Server-connection`  

<div style="clear:both"> </div>

Returns all status attributes of channels which matches the given name. The 
data will be returned in a JSON array as the `statuses` property of the JSON 
object. When a WebSphere MQ error occurred an `error` property is set and the 
`statuses` property is not set.
