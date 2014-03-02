---
layout: default
title: MQWeb &bull; API
api: true
---

MQWeb API
=========

The api controllers are responsible of sending PCF commands to WebSphere MQ 
and to return the answer in JSON format. Each WebSphere MQ object will have 
its own controller. The URI determines which controller will handle the request
and which action will be called. The URI always starts with `api`, the second 
part of the URI selects the controller and the third part defines the action. 
For example: the URI `/api/queue/inquire` will call the inquire action on the 
`QueueController`. An action needs some parameters to know which WebSphere MQ 
object to use. These parameters are passed as part of the path of the URI. 
The first parameter is always the name of the queuemanager. Other parameters 
depends on the called action. For example: `/api/queue/inquire/PIGEON` will 
return all queues from the queuemanager PIGEON. Properties of WebSphere MQ 
objects are returned with the name as documented in the WebSphere MQ information 
center. For example: The property of the current queue depth of a queue will 
have the name `CurrentQDepth`.

|Controller|URI path|
|----------|--------|
|[ChannelController](channel.html)|/api/channel|
|[ChannelStatusController](chstatus.html)|/api/chstatus|
|[QueueManagerController](qmgr.html)|/api/qmgr|
|[QueueManagerStatusController](qmstatus.html)|/api/qmstatus|
|[QueueController](queue.html)|/api/queue|
|[ListenerController](listener.html)|/api/listener|
|[ListenerStatusController](lsstatus.html)|/api/lsstatus|
|[MessageController](message.html)|/api/message|

