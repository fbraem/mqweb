---
layout: default
title: MQWeb &bull; QueueController
features: true
hideSidebar: true
---
QueueController
===============

The first part of the URI must be `queue` to call the QueueController.
This controller can be used to get information from a queue.

##Actions

###index

Name
: index

Type
: HTML

Parameters
: + **queueManager** 
    The name of the queuemanager

Example
: `/queue/index/PIGEON`

Shows a form to inquire a queuelist. It's possible to directly go to the detail 
page of a queue. Pass the name of queue as fragment of the URI. MQWeb uses the 
AngularJS [ngRoute](http://docs.angularjs.org/api/ngRoute) module to make this
work. `/queue/index/PIGEON/#MQWEB.TEST.Q01` will show the detail page of queue
`MQWEB.TEST.Q01`.

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
: `queue/inquire/PIGEON/MQWEB.TEST.Q1`  
  `queue/inquire/PIGEON/*`  
  `queue/inquire/PIGEON?queueName=*&queueDepth=1`

When a queuename parameter is passed in the URI path only the attributes of the
queues which matches the given name will be returned. Query parameters are
discarded.

A filter will be created based on the query parameters when no queuename parameter is passed in the URI path.

The returned JSON object will have a `mqweb` object and a `queues` array. 
When a WebSphere MQ error occurred there will be no `queues` array, but instead an `error` object is returned.
