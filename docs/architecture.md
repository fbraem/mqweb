---
layout: default
title: MQWeb &bull; Architecture
doc_architecture: true
---
Architecture
============

> The documented features are part of the upcoming release 0.0.9
> This page is still a work in progress ...

This sequence diagram shows how a built-in HTML page is build.

<img src="{{ site.baseurl }}/img/mqwebsequence.png" style="text-align:center" alt="MQWeb Sequence Diagram" />

MQWeb uses the HTTPServer class from [POCO](http://www.pocoproject.org). 
This class uses request handlers to handle incoming requests. MQWeb has three 
handlers.

##StaticRequestHandler

This request handler is called when the URI starts with `static` and is only
used when the built-in HTML pages are used. This handler is responsible for
returning style sheets, images, scripts, ... In other words: it returns static
content.

##DenyRequestHandler

This handler is called when MQWeb detects an incoming request that needs to
be blocked. It will always return 403 Forbidden.
  
##ControllerRequestHandler

This handler is responsible for creating the controller that handles a request
for a WebSphere MQ object. There are two types of controllers: the web controller
and the api controller. The web controller is responsible of returning the
HTML page for a WebSphere MQ object. The web controller will be called when the
first part of the URI is `web`. The api controllers are responsible of sending 
PCF commands to WebSphere MQ and to return the answer in JSON format. Each
WebSphere MQ object has its own api controller. When the URI starts with `api`, 
the second part of the URI will determine which api controller will be called. 
For example: the URI `/api/queue/inquire` will call the inquire action on the 
`QueueController`. An action needs some parameters to know which WebSphere MQ 
object to use. These parameters are passed as part of the path of the URI. 
The first parameter is always the name of the queuemanager. Other parameters 
depends on the called action. For example: `/api/queue/inquire/PIGEON` will 
return all queues from the queuemanager PIGEON. Properties of WebSphere MQ 
objects are returned with the name as documented in the WebSphere MQ information 
center. For example: The property of the current queue depth of a queue will 
have the name `CurrentQDepth`. Some api's also allow the use of Query 
parameters. More information on each api can be find on the api pages.
