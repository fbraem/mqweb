---
layout: default
title: MQWeb &bull; Architecture
doc_architecture: true
---
Architecture
============

MQWeb uses the HTTPServer class from [POCO](http://www.pocoproject.org).
This class uses request handlers to handle incoming requests. MQWeb has four
handlers.

## AppRequestHandler

This request handler is called when a web application is used. The URI starts 
with `web` and returns the content based on the `mq.web.app` property.

## DenyRequestHandler

This handler is called when MQWeb detects an incoming request that needs to
be blocked. It will always return 403 Forbidden.

## ControllerRequestHandler

This handler is responsible for creating the controller that handles a request
for a WebSphere MQ object. The [api controllers](/api/index.html) are 
responsible of sending PCF commands to WebSphere MQ and to return the answer in 
JSON format. An api controller is called when the URI begins with `api`.

## WebSockeRequestHandler

This handler uses a Websocket to forward messages from a queue to the client. 
**This is currently experimental code**!