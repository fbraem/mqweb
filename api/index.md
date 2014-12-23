---
layout: default
title: MQWeb &bull; API
api: true
---

MQWeb API
=========

The api controllers are responsible of sending PCF commands to WebSphere MQ 
and to return the answer in JSON format.

Each WebSphere MQ object type will have its own controller. The URI determines 
which controller will handle the request and which action will be called. The 
URI always starts with `api`, the second part of the URI selects the controller 
and the third part defines the action. For example: the URI `/api/queue/inquire` 
will call the [inquire](queue.html#inquire) action on the 
[QueueController](queue.html). 

An action needs some parameters to know which WebSphere MQ object to use. These 
parameters are passed as part of the path of the URI or as query parameter. 
The first parameter on the URI after the action is always the name of the 
queuemanager. Other parameters depends on the called action.
For example: `/api/queue/inquire/PIGEON` will return all queues from the queuemanager 
PIGEON. Depending on the object type other input can be passed. Look at the api
documentation to know what parameters can be used. The HTTP method can be GET or POST.

Query parameter names are, as much as possible, based on the names used in the
WebSphere MQ information center. For example: `QName` will be used for a queuename.
The name of a query parameter is not case sensitive.

It's also possible to POST an application/json request. The URI is still formed
with the object type, action and queuemanager name but all other information can be
posted as a JSON object. This JSON object contains input information for the PFC 
command. Look at the api documentation to know what properties can be used in 
this object. The name of a property is likely the same as the corresponding 
query parameter but is now case sensitive!

The answer of an api request is always a JSON object. This JSON object can contain the
following properties:

+ `mqweb`

  Contains general information about the api call.

+ `filter`

  Contains the data which is used as input for the PCF command. This can be 
  useful to debug a GET/POST request.

+ `error`

  Only returned when a WebSphere MQ error occurred.

+ An array or object depending on the WebSphere MQ object type (only returned when 
  no error occurred).

Properties of WebSphere MQ objects are always returned with the name as documented 
in the WebSphere MQ information center. For example: The property of the current 
queue depth of a queue will have the name `CurrentQDepth`. Each property is an
object with the following properties:

+ `id`

  The id of the WebSphere MQ property. For example `CurrentQDepth` will have the
  id `3`, which is the equivalent of the C constant `MQIA_CURRENT_Q_DEPTH`.

+ `value`

  The value of the WebSphere MQ property.

+ `display`

  When the value of the property is an MQ constant then this property will have
  the string representation of the value. For example: When `QueueType` has the
  value 1 (MQQT_LOCAL), then this property will have the value `Local`. 

> A property with the name `id_nnnn` (where nnnn is the value of the C constant) 
> means that the property is not defined in the SQLite database. Create an 
> issue on [Github](https://github.com/fbraem/mqweb/issues) and this will be 
> fixed as soon as possible.

Controllers
-----------

|Controller|URI path|
|----------|--------|
|[AuthenticationInformationController](authinfo.html)|/api/authinfo|
|[AuthorityRecordController](authrec.html)|/api/authrec|
|[ChannelController](channel.html)|/api/channel|
|[ChannelInitiatorController](chinit.html)|/api/chinit|
|[ChannelStatusController](chstatus.html)|/api/chstatus|
|[ClusterQueueManagerController](clusqmgr.html)|/api/clusqmgr|
|[ConnectionController](conn.html)|/api/conn|
|[QueueManagerController](qmgr.html)|/api/qmgr|
|[QueueManagerStatusController](qmstatus.html)|/api/qmstatus|
|[QueueController](queue.html)|/api/queue|
|[ListenerController](listener.html)|/api/listener|
|[ListenerStatusController](lsstatus.html)|/api/lsstatus|
|[MessageController](message.html)|/api/message|
|[TopicController](topic.html)|/api/topic|
|[TopicStatusController](tpstatus.html)|/api/tpstatus|
|[SubController](sub.html)|/api/sub|

