---
layout: default
title: MQWeb &bull; QueueController
api: true
hideSidebar: true
---
QueueController
===============

The second part of the URI must be `queue` to call the QueueController.
At this time, this controller can be used to [inquire](#inquire) information from a queue.

##<a name="inquire"></a>inquire
Get information about one or more queues. This action executes the MQCMD_INQUIRE_Q pcf command.

The returned JSON object will have a `queues` array. When a WebSphere MQ error occurred 
there will be no `queues` array, but instead an `error` object is returned.

###<a name="inquireUrl"></a>URL Parameters
`/api/queue/inquire/<QueueManager>/<QName>`

####<a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

####<a name="inquireUrlQName"></a>QName
The name of a queue. Generic queue names are supported.
This parameter is optional.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryCFStructure"></a>CFStructure
Specifies the name of the storage class. This parameter is valid only on z/OS.

####<a name="inquireQueryClusterInfo"></a>ClusterInfo
When `true` cluster information about queues and other queues in the repository 
that match the selection criteria is returned. By default this is `false`.

####<a name="inquireQueryCurrentQDepth"></a>CurrentQDepth
Only return queues which have at least *CurrentQDepth* messages. This is
actually a shortcut for a [Filter](#inquireFilter) : 

    filter=I&filter_param=CurrentQDepth&filter_op=NLT&filter_value=<value>

For compatibility reasons with older versions this parameter can also
be passed as *QueueDepth*.

> Be aware that only one integer/string filter can be used for each call.

####<a name="inquireQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, all queues starting with SYSTEM. will be discarded.
This parameter is optional. By default the value is set to `false`.

####<a name="inquireQueryExcludeTemp"></a>ExcludeTemp
When value is `true`, all temporary queues will be discarded.
This parameter is optional. By default the value is set to `false`.

> When the [QAttrs](#inquireQueryQAttrs) parameter is used, the DefinitionType attribute 
> must be selected to make this work.

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

####<a name="inquireQueryQAttrs"></a>QAttrs
With the QAttrs parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter
are possible. The value must be a valid attribute name.

> Attrs is a synonym for QAttrs

####<a name="inquireQueryQName"></a>QName
Only return queues with a name that matches *QName*. By 
default * is used which matches all queues. *Name* is a synonym for this 
parameter.

This parameter is ignored when there is a URI parameter for a queuename.

>For compatibility reasons with older versions this parameter can also
>be passed as *QueueName*.

####<a name="inquireQueryQType"></a>QType
Only return the queues of the given type. Possible values are `Local`,
`Remote`, `Model`, `Alias`, `Cluster` or `All`. Default is `All`. The
value is case-sensitive.

For compatibility reasons with older versions this parameter can also
be passed as *QueueType*.

####<a name="inquireQueryUsage"></a>Usage
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

###<a name="inquireExample"></a>Example

`/api/queue/inquire/PIGEON/MQWEB.TEST.Q1`  
`/api/queue/inquire/PIGEON/*`  
`/api/queue/inquire/PIGEON?QName=*&CurrentQDepth=1`  
`/api/queue/inquire/PIGEON?QAttrs=QName&QAttrs=CurrentQDepth`

This sample is a PHP script that inquires all SYSTEM queues from the PIGEON
queuemanager:

{% highlight php %}
<?php

	/*
	 * Inquire all SYSTEM queues from queuemanager PIGEON.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/queue/inquire/PIGEON/SYSTEM*";
	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_HEADER, false);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
	$response = curl_exec($curl);
	$data = json_decode($response, true);
	print_r($data);
{% endhighlight %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with names like the
query parameters.

> All URL parameters and query parameters are ignored except for the URL parameter for
> the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
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
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ *QAttrs* is a JSON array with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be *QName* for example.
+ *CurrentQDepth* can't be used. You need to use an *IntegerFilterCommand* to do the same.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter ([FilterParam](#inquireQueryFilterParam)), 
  Operator ([FilterOp](#inquireQueryFilterOp)) and FilterValue ([FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all queues starting with `T`:

{% include_relative samples/queue.html %}

