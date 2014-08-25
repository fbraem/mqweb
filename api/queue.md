---
layout: default
title: MQWeb &bull; QueueController
api: true
hideSidebar: true
---
QueueController
===============

The second part of the URI must be `queue` to call the QueueController.
This controller can be used to get information from a queue.

##inquire

Get information about one or more queues. When a queuename parameter is passed in the 
URI path only the attributes of the queues which matches the given name will be 
returned. Query parameters are discarded in this case.

A filter will be created based on the query parameters when no queuename 
parameter is passed in the URI path.

The returned JSON object will have a `mqweb` object and a `queues` array. 
When a WebSphere MQ error occurred there will be no `queues` array, but instead 
an `error` object is returned.

###URL Parameters

`/api/queue/inquire/<QueueManager>/<QueueName>`

**QueueManager**
 
The name of the queuemanager. This parameter is required.

**QueueName** (optional)

The name of a queue.

### Query Parameters

**QueueDepth** (optional)

Only return queues which have at least *QueueDepth* messages.

**QueueName** (optional)

Only return queues with a name that matches *QueueName*. By 
default * is used which matches all queues.

**QueueType** (optional)
  
Only return the queues of the given type. Possible values are `Local`,
`Remote`, `Model`, `Alias`, `Cluster` or `All`. Default is `All`. The
value is case-sensitive.

**QueueUsage** (optional)

Only return queues with the given usage type. Use `normal` or `xmitq`. When
this parameter is not set, all queues will be returned.

**ExcludeSystem** (optional)

When value is `true`, all queues starting with SYSTEM. will be discarded.

**ExcludeTemp** (optional)

When value is `true`, all temporary queues will be discarded.

###Example

`/api/queue/inquire/PIGEON/MQWEB.TEST.Q1`  
`/api/queue/inquire/PIGEON/*`  
`/api/queue/inquire/PIGEON?QueueName=*&QueueDepth=1`
