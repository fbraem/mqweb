---
layout: default
title: MQWeb &bull; ListenerStatusController
api: true
---
ListenerStatusController
========================

The second part of the URI must be `lsstatus` to call the 
ListenerStatusController.

##<a name="inquire"></a>inquire
Get status information about one ore more listeners. This actions executes the
PCF commando MQCMD_INQUIRE_LISTENER_STATUS.The returned JSON object will have a 
`mqweb` object and a `statuses` array with all returned statuses as object. 
When a WebSphere MQ error occurred, there will be no `statuses` array.
Instead an `error` object is returned.

###<a name="inquireURL"></a>URL Parameters
`/api/lsstatus/inquire/<QueueManager>/<ListenerName>`

####<a name="inquireURLQueueManager"></a>Queuemanager
The name of the queuemanager. This parameter is required.

####<a name="inquireURLListenerName"></a>ListenerName
The name of the listener. When this parameter is used, the query parameter for 
[listername](#inquireQueryListenerName) will be ignored. A generic name can be
used.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, the status of system listeners are not returned. By 
default this parameter is `false`. This parameter is optional.

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

####<a name="inquireQueryListenerName"></a>ListenerName
The name of the listener. *name* is a synonym for this parameter. It is ignored
when a [URL parameter](#inquireURLListenerName) is used. Generic names are 
allowed.

####<a name="inqueryQueryListenerStatusAttrs"></a>ListenerStatusAttrs
With the *ListenerStatusAttrs* parameter you can specify which attributes must 
be returned from the PCF command. Multiple occurences of this parameter are 
possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for ListenerStatusAttrs
    
###<a name="inquireExample"></a>Example
`/api/lsstatus/inquire/PIGEON`  
`/api/lsstatus/inquire/PIGEON?ListenerName=MQWEB*`

{% include_relative samples/php/lsstatus_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
{
  'ListenerName' : 'SYSTEM*',
  'ListenerStatusAttrs' : [
    'ListenerName'
  ]
}
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ [ListenerStatusAttrs](#inquireQueryListenerStatusAttrs) is a JSON array with 
  attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be 
  [ListenerName](#inquireQueryListenerName) for example.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter ([FilterParam](#inquireQueryFilterParam)), 
  Operator ([FilterOp](#inquireQueryFilterOp)) and FilterValue ([FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This is a Perl example that inquires all statuses.

{% include_relative samples/perl/lsstatus_inq.html %}

