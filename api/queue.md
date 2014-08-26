---
layout: default
title: MQWeb &bull; QueueController
api: true
hideSidebar: true
---
QueueController
===============

The second part of the URI must be `queue` to call the QueueController.
This controller can be used to get information from a queue.

##inquire

Get information about one or more queues. This action executes the MQCMD_INQUIRE_Q pcf command.

The returned JSON object will have a `queues` array. When a WebSphere MQ error occurred 
there will be no `queues` array, but instead an `error` object is returned.

###URL Parameters

`/api/queue/inquire/<QueueManager>/<QName>`

####QueueManager
 
The name of the queuemanager. This parameter is required.

####QName

The name of a queue. Generic queue names are supported.
This parameter is optional.

###Query Parameters

####CurrentQDepth

Only return queues which have at least *CurrentQDepth* messages. This is
actually a shortcut for an *IntegerFilterCommand* : 

    {
      'Parameter' : 'CurrentQDepth',
      'Operator' : 'NLT',
      'FilterValue' : <value>
    }

For compatibility reasons with older versions this parameter can also
be passed as *QueueDepth*.

> Be aware that only one integer/string filter can be used for each call.

####ExcludeSystem

When value is `true`, all queues starting with SYSTEM. will be discarded.
This parameter is optional. By default the value is set to `false`.

####ExcludeTemp

When value is `true`, all temporary queues will be discarded.
This parameter is optional. By default the value is set to `false`.

####QAttrs

With the QAttrs parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter
are possible. The value must be a valid attribute name.

> Attrs is a synonym for QAttrs

####QName

Only return queues with a name that matches *QName*. By 
default * is used which matches all queues.

This parameter is ignored when there is a URI parameter for a queuename.

For compatibility reasons with older versions this parameter can also
be passed as *QueueName*.

####QType
  
Only return the queues of the given type. Possible values are `Local`,
`Remote`, `Model`, `Alias`, `Cluster` or `All`. Default is `All`. The
value is case-sensitive.

For compatibility reasons with older versions this parameter can also
be passed as *QueueType*.

####Usage

Only return queues with the given usage type. Use `normal` or `xmitq`. When
this parameter is not set, all queues will be returned. The value is not
case-sensitive.

> This parameter doesn't actually exist for MQCMD_INQUIRE_Q, but because it
> is useful to only get information for transmission queues this
> parameter is added.

> When the QAttrs parameter is used, the Usage attribute must be selected to
> make this work.

For compatibility reasons with older versions this parameter can also
be passed as *QueueUsage*.

###Example

`/api/queue/inquire/PIGEON/MQWEB.TEST.Q1`  
`/api/queue/inquire/PIGEON/*`  
`/api/queue/inquire/PIGEON?QName=*&CurrentQDepth=1`  
`/api/queue/inquire/PIGEON?QAttrs=QName&QAttrs=CurrentQDepth`

###JSON Object

When using a JSON POST request you can post a JSON object with names like the
query parameters. *QAttrs* is a JSON array with attributenames as element.

These are some additional parameters:

####IntegerFilterCommand

####StringFilterCommand

