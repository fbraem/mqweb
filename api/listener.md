---
layout: default
title: MQWeb &bull; ListenerController
api: true
---
ListenerController
==================

The second part of the URI must be `listener` to call the ListenerController.

##<a name="inquire"></a>inquire
Get information about one ore more listeners. This actions executes the
PCF commando MQCMD_INQUIRE_LISTENER.

The returned JSON object will have a `listeners` property which contains all
matching listeners. When a WebSphere MQ error occurred there will be no 
`listeners` array, but instead an `error` object is returned.

###<a name="inquireURL"></a>URL Parameters
`/api/listener/inquire/<QueueManager>/<ListenerName>`

####<a name="inquireURLQueueManager"></a>QueueManager

The name of the queuemanager. This parameter is required!

####<a name="inquireURLListenerName"></a>ListenerName
  
The name of the listener. When this parameter is used, the 
[ListerName](#inquireQueryListenerName) query parameter is ignored. This 
parameter is optional. Generic names can be used.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, system listeners are not returned. By default this
parameter is `false`. This parameter is optional.

####<a name="inquireQueryFilter"></a>Filter
Speficies which filter to use: `I` means Integerfilter, `S` means Stringfilter.
*FilterParam* and *FilterValue* are required to create the filter. When a filter can't be build
because of too little information, it will be silently discarded.

####<a name="inquireQueryFilterOp"></a>FilterOp
{% capture filterop %}{% include filterop.md %}{% endcapture %}
{{ filterop | markdownify }}

####<a name="inquireQueryFilterParam"></a>FilterParam
The name of the parameter to filter on. The names are based on the names used in the WebSphere MQ information center.

####<a name="inquireQueryFilterValue"></a>FilterValue
The value to use for filtering. When a string is passed for an Integerfilter, a WebSphere MQ constant is assumed.

####<a name="inqueryQueryListenerAttrs"></a>ListenerAttrs
With the *ListenerAttrs* parameter you can specify which attributes must be 
returned from the PCF command. Multiple occurences of this parameter are 
possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for ListenerAttrs

####<a name="inquireQueryListenerName"></a>ListenerName
The name of the listener. Generic names can be used. When an 
[listenername](#inquireURLListenerName) is passed as part of the URL, this 
query parameter will be ignored. *name* is a synonym for this parameter.

####<a name="inquireQueryTransportType"></a>TransportType
Only return listeners with the given transport type. The following types are 
possible: `All`, `Local`, `LU62`, `TCP`, `NetBIOS`, `SPX`, `DECnet` or `UDP`. 
The default is `All`. The value is case-sensitive.

###<a name="inquireExample"></a>Example
`/api/listener/inquire/PIGEON`  
`/api/listener/inquire/PIGEON?listerName=*&listenerExcludeSystem=false`  

This sample is a PHP script that inquires all SYSTEM listeners with 
transporttype TCP from queuemanager PIGEON:

{% include_relative samples/php/listener_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
{
  'ListenerName' : 'SYSTEM*',
  'ListenerAttrs' : [
    'ListenerName',
    'TransportType'
  ]
}
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ [ListenerAttrs](#inquireQueryListenerAttrs) is a JSON array with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be 
  [ListenerName](#inquireQueryListenerName) for example.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter ([FilterParam](#inquireQueryFilterParam)), 
  Operator ([FilterOp](#inquireQueryFilterOp)) and FilterValue ([FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This is a Perl example that inquires all listeners and returns the listenername
and transportype attributes.

{% include_relative samples/perl/listener_inq.html %}

