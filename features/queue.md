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
: **queueManager:** The name of the queuemanager

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
: **queueManager:** The name of the queuemanager

Query Parameters
: + **queueDepth** 
    Only return queues which have at least *queueDepth* messages.
  + **queueName** 
    Only return queues with a name that matches *queueName*. By 
    default * is used which matches all queues.

Example
: `queue/index/PIGEON`

Returns a JSON object with all queues matching the filter. The filter is created
based on the values passed in the query parameters. The returend JSON object
will have a `mqweb` object and a `queues` array. When a WebSphere MQ error
occurred there will be no `queues` array, but instead an `error` object is
returned.
