---
layout: default
title: MQWeb use case
excerpt: MQWeb use case
---
With some pride I can announce today that my employer, [KBC](http://www.kbc.be), 
is now using my open source project MQWeb to help their developers, who use 
WebSphere MQ, to get status information from their queues, channels, ...
 
An internal website is created with CakePHP to get an overview of all available
queuemanagers. Once a queuemanager is selected, AngularJS is used to get
information from the MQWeb daemon. AngularJS is also used to turn the JSON data
into HTML.
