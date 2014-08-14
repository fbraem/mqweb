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

##Actions

###inquire

####Parameters

**queueManager**

The name of the queuemanager

**channelName** (optional)
  
The name of a channel. When this parameter is used, query parameters are ignored.

#####Query Parameters
**channelType** (optional)

**channelName** (optional)

#####Example
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
