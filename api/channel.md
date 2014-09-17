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

##<a name="inquire"></a>inquire

###<a name="inquireURL"></a>URL Parameters
`/api/channel/inquire/<QueueManager>/<ChannelName>/<ChannelType>`

####<a name="inquireURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required!

####<a name="inquireURLChannelName"></a>ChannelName
The name of a channel. Generic names are supported. When this parameter is used, 
query parameters are ignored. This parameter is optional.

####<a name="inquireURLChannelType"></a>ChannelType
The type of the channel. ChannelType must be `All`, `Sender`, `Server`, `Receiver`, `Requester`, `Server-connection`, `Client-connection`, `Cluster-receiver` or `Cluster-sender`. This value is case-sensitive.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryChannelAttrs"></a>ChannelAttrs
With the *ChannelAttrs* parameter you can specify which attributes must be returned from the PCF command. Multiple occurences of this parameter are possible. The value must be a (case-sensitive) valid attribute name.

####<a name="inquireQueryChannelName"></a>ChannelName
The name of a channel. Generic names are supported. This parameter is ignored when
a [channelname](#inquireURLChannelName) is passed as URL parameter. When this parameter
is omitted, * will be used.

####<a name="inquireQueryChannelType"></a>ChannelType
ChannelType must be `All`, `Sender`, `Server`, `Receiver`, `Requester`, `Server-connection`, `Client-connection`, `Cluster-receiver` or `Cluster-sender`. This value is case-sensitive. When channel type is passed as 
[URL parameter](#inquireURLChannelType), this query parameter is ignored.

####<a name="inquireQueryCommandScope"></a>CommandScope

####<a name="inquireQueryQSGDisposition"></a>QSGDisposition

###Example
`/api/channel/inquire/PIGEON`  
`/api/channel/inquire/PIGEON/CL.*`  
`/api/channel/inquire/PIGEON?ChannelName=CL.*`

A filter will be created based on the query parameters when no channelname 
parameter is passed in the URI path.

The returned JSON object will have a `mqweb` object and a `channels` array. 
When a WebSphere MQ error occurred there will be no `channels` array, but instead 
an `error` object is returned.
