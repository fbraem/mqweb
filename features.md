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
channels, messages, ... When the build-in HTML pages are used, AngularJS will 
be used to transform the JSON model objects into a view in the browser. 

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
  WebSphere MQ object to use. The first parameter is always the name of the 
  queuemanager. Other parameters depends on the called action and are documented
  on this page. For example: `/queue/inquire/PIGEON` will return all queues
  from the queuemanager PIGEON.

##Queuemanager

The URI must start with `qmgr` to call the QueueManagerController.

| URL | Type | Parameters | Description |
| --- | ---- | ---------- | ----------- |
| qmgr/index/&lt;qmgrName&gt; | HTML | | Shows a dashboard page for a queuemanager |

##Queue

The controller for a queue is *queue*

| URL | Type | Parameters | Description |
| --- | ---- | ---------- | ----------- |
| queue/index/&lt;qmgrName&gt; | HTML | | Shows a form for inquiring queues |
| queue/index/&lt;qmgrName&gt;/&lt;queueName&gt; | HTML | | Shows a detail page for a queue |
| queue/inquire/&lt;qmgrName&gt;|JSON| |Inquires the queues |


