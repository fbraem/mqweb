---
layout: default
title: MQWeb &bull; ListenerController
api: true
hideSidebar: true
---
ListenerController
==================

The second part of the URI must be `listener` to call the ListenerController.
This controller can be used to get information from a listener.

##Actions

###inquire

Name
: inquire

Type
: JSON

Parameters
: + **queueManager**

    The name of the queuemanager

  + **listenerName** (optional)
  
    The name of the listener. When this parameter is used, the query parameters
    are ignored.

Query Parameters
: +  **listenerName** (optional)

    Only return listeners with a name that matches *listenerName*. By 
    default * is used which matches all listeners.

  + **listenerType** (optional)
  
    Only return listeners of the given type. The following types are possible:
    `All`, `Local`, `LU62`, `TCP`, `NetBIOS`, `SPX`, `DECnet` or `UDP`. The
    default is `All`

  + **listenerExcludeSystem** (optional)
  
    When value is `true`, system listeners are not returned. By default this
    parameter is `false`.

Example
: `/api/listener/inquire/PIGEON`  
  `/api/listener/inquire/PIGEON?listerName=*&listenerExcludeSystem=false`  

<div style="clear:both"> </div>

Returns all attributes of the matching listeners. The data will be returned as 
a JSON array in the `listeners` property of the JSON object. When a WebSphere MQ 
error occurred an `error` property is set and the `listeners`  property is not 
set.
