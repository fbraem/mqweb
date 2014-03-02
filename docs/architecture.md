---
layout: default
title: MQWeb &bull; Architecture
doc_architecture: true
---
Architecture
============

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
for a WebSphere MQ object. There are two types of controllers:

+ The [web controller](docs/web.html) is responsible of returning a built-in 
HTML page for a WebSphere MQ object. The web controller will be called when the 
first part of the URI is `web`. 
+ The [api controllers](/api/index.html) are responsible of sending PCF commands 
to WebSphere MQ and to return the answer in JSON format. An api controller is 
called when the URI begins with `api`. 

