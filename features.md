---
layout: default
title: MQWeb &bull; Features
features: true
---
Features
========

> The documented features are part of the upcoming release 0.0.9
> This page is still a work in progress ...

##Queuemanager

The controller for getting information for a queuemanager is *qmgr*

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


