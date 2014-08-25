---
layout: default
title: MQWeb &bull; ChannelController
api: true
hideSidebar: true
---
ChannelController
=================

The second part of the URI must be `channel` to call the ChannelController.
This controller can be used to get information from a channel.

##inquire

###URL Parameters

`/api/channel/inquire/<QueueManager>/<ChannelName>`

**QueueManager**

The name of the queuemanager. This parameter is required!

**ChannelName** (optional)
  
The name of a channel. When this parameter is used, query parameters are ignored.

###Query Parameters

**ChannelType** (optional)

ChannelType must be `All`, `Sender`, `Server`, `Receiver`, `Requester`, `Server-connection`, `Client-connection`, `Cluster-receiver` or `Cluster-sender`. This value is case-sensitive. When this parameter is omitted, `All` is used.

**ChannelName** (optional)

###Example
`/api/channel/inquire/PIGEON`  
`/api/channel/inquire/PIGEON/CL.*`  
`/api/channel/inquire/PIGEON?channelName=CL.*`

<div style="clear:both"> </div>

When a channelname parameter is passed in the URI path only the attributes of the
channels which matches the given name will be returned. Query parameters are
discarded in this case.

A filter will be created based on the query parameters when no channelname 
parameter is passed in the URI path.

The returned JSON object will have a `mqweb` object and a `channels` array. 
When a WebSphere MQ error occurred there will be no `channels` array, but instead 
an `error` object is returned.
