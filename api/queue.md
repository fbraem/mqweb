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

##Actions

###inquire

Name
: inquire

Type
: JSON

Parameters
: + **queueManager**
 
    The name of the queuemanager

  + **queueName** (optional)

    The name of a queue

Query Parameters
: + **queueDepth** (optional)

    Only return queues which have at least *queueDepth* messages.

  + **queueName** (optional)

    Only return queues with a name that matches *queueName*. By 
    default * is used which matches all queues.

  + **queueUsage** (optional)

    Only return queues with the given usage type. Use `normal` or `xmitq`. When
    this parameter is not set, all queues will be returned.

  + **excludeSystem** (optional)

    When value is `true`, all queues starting with SYSTEM. will be discarded.

  + **excludeTemp** (optional)

    When value is `true`, all temporary queues will be discarded.

Example
: `/api/queue/inquire/PIGEON/MQWEB.TEST.Q1`  
  `/api/queue/inquire/PIGEON/*`  
  `/api/queue/inquire/PIGEON?queueName=*&queueDepth=1`

<div style="clear:both;"> </div>

When a queuename parameter is passed in the URI path only the attributes of the
queues which matches the given name will be returned. Query parameters are
discarded in this case.

A filter will be created based on the query parameters when no queuename 
parameter is passed in the URI path.

The returned JSON object will have a `mqweb` object and a `queues` array. 
When a WebSphere MQ error occurred there will be no `queues` array, but instead 
an `error` object is returned.
