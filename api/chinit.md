---
layout: default
title: MQWeb &bull; ChannelInitiatorController
api: true
---
ChannelInitiatorController
==========================
The second part of the URI must be `chinit` to call the
 ChannelInitiatorController. This controller is only useable for queuemanagers
 running on z/OS.

##<a name="inquire"></a>inquire
Get info about the channel initiator. This api executes the MQCMD_INQUIRE_CHANNEL_INIT
pcf command.

The returned JSON object will have a `response` array.

###<a name="inquireUrl"></a>URL Parameters
`/api/chinit/inquire/<QueueManager>`

####<a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queuemanager is a member of a 
queue-sharing group. This parameter applies to z/OS only.

###<a name="inquireExample"></a>Example

`/api/chinit/inquire/PIGEON`  

This sample is a PHP script that inquires the channel initiator:

{% include_relative samples/php/chinit_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive

This sample is a Perl script that inquires the channel initiator:

{% include_relative samples/perl/chinit_inq.html %}
