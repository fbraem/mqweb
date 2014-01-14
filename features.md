---
layout: default
title: MQWeb &bull; Features
features: true
---
Features
========

> The documented features are part of the upcoming release 0.0.9
> This page is still a work in progress ...

MQWeb uses the MVC pattern. Models are JSON objects that represents queues,
channels, messages and other WebSphere MQ objects. When the build-in HTML pages 
are used, [AngularJS](http://angularjs.org) is used to transform the JSON model 
objects into a view in the browser. 

MQWeb uses the HTTPServer class from [POCO](http://www.pocoproject.org). 
This class uses request handlers to handle incoming requests. MQWeb has three 
handlers:

+ StaticRequestHandler

  This request handler is called when the URI starts with `static` and is only
  used when the build-in HTML pages are used. This handler is responsible for
  returning style sheets, images, scripts, ... In other words: it returns static
  content.

+ DenyRequestHandler

  This handler is called when MQWeb detects an incoming request that needs to
  be blocked. It will always return 403 Forbidden.
  
+ ControllerRequestHandler

  This handler is responsible for creating the controller that handles a request
  for a WebSphere MQ object. Each type of WebSphere MQ object has its own
  controller. Each controller has actions that are called based on the URI. For
  example: the URI `/queue/inquire` will call the inquire action on the 
  `QueueController`. An action needs some parameters to know which
  WebSphere MQ object to use. These parameters are passed as path of the URI.
  The first parameter is always the name of the queuemanager. Other parameters 
  depends on the called action and are documented on this page. For example: 
  `/queue/inquire/PIGEON` will return all queues from the queuemanager PIGEON. 
  Properties of WebSphere MQ objects are returned with the name as documented 
  in the WebSphere MQ information center. For example: The property of the 
  current queue depth of a queue will have the name `CurrentQDepth`.

##Queuemanager

The first part of the URI must be `qmgr` to call the QueueManagerController.

###index

Name
: index

Type
: HTML

Parameters
: **queueManager:** The name of the queuemanager

Example
: `/qmgr/index/PIGEON`

Shows a dashboard page for a queuemanager

##Queue

The first part of the URI must be `queue` to call the QueueController.

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
: **queueDepth:** Only return queues which have at least *queueDepth* messages.
  **queueName:** Only return queues with a name that matches *queueName*. By 
  default * is used which matches all queues.

Example
: `queue/index/PIGEON`

Returns a JSON object with all queues matching the filter. The filter is created
based on the values passed in the query parameters. The returend JSON object
will have a `mqweb` object and a `queues` array. When a WebSphere MQ error
occurred there will be no `queues` array, but instead an `error` object is
returned.
