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

<a name="urlQueueManager"></a>
####QueueManager

The name of the queuemanager. This parameter is required.

####QName

The name of a queue. Generic queue names are supported.
This parameter is optional.

###Query Parameters

####CurrentQDepth

Only return queues which have at least *CurrentQDepth* messages. This is
actually a shortcut for a *Filter* : 

    filter=I&filter_param=CurrentQDepth&filter_op=NLT&filter_value=<value>

For compatibility reasons with older versions this parameter can also
be passed as *QueueDepth*.

> Be aware that only one integer/string filter can be used for each call.

####ExcludeSystem

When value is `true`, all queues starting with SYSTEM. will be discarded.
This parameter is optional. By default the value is set to `false`.

####ExcludeTemp

When value is `true`, all temporary queues will be discarded.
This parameter is optional. By default the value is set to `false`.

<a name="queryFilter"></a>
####Filter

Speficies which filter to use: `I` means Integerfilter, `S` means Stringfilter.
*FilterParam* and *FilterValue* are required to create the filter. When a filter can't be build
because of too little information, it will be silently discarded.

<a name="queryFilterOp"></a>
####FilterOp

The operator that is being used to evaluate whether the parameter satisfies the filter-value.
The default is `EQ`.

The following values are allowed:

+ `GT` : Greater than
+ `LT` : Less than
+ `EQ` : Equal to
+ `NE` : Not equal to
+ `NLT` : Not less than
+ `NGT` : Not greater than
+ `LE` : Less than or equal to
+ `GE` : Greater than or equal to
+ `CT` : Contains
+ `EX` : Excludes

A Stringfilter can use some additional operators:

+ `LK` : Matches a generic string
+ `NL` : Does not match a generic string
+ `CTG` : Contains an item which matches a generic string
+ `EXG` : Does not contain any item which matches a generic string.

<a name="queryFilterParam"></a>
####FilterParam

The name of the parameter to filter on. The names are based on the names used in the WebSphere MQ information center.

<a name="queryFilterValue"></a>
####FilterValue

The value to use for filtering. When a string is passed for an Integerfilter, a WebSphere MQ constant is assumed.

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

When using an application/json POST request you can post a JSON object with names like the
query parameters.

> All URL parameters and query parameters are ignored except for the URL parameter for
> the name of the [queuemanager](#urlQueueManager).

    {
      'QName' : 'T*',
      'QAttrs' : [
        'QName',
        'CurrentQDepth'
      ],
      'IntegerFilterCommand' : {
        'Parameter' : 'CurrentQDepth',
        'Operator' : 'NLT',
        'FilterValue' : 1
      }
    }

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ *QAttrs* is a JSON array with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be *QName* for example.
+ *CurrentQDepth* can't be used. You need to use an *IntegerFilterCommand* to do the same.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter ([FilterParm](#queryFilterParm)), 
  Operator ([FilterOp](#queryFilterOp)) and ([FilterValue](#queryFilterValue])).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*
