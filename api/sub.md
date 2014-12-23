---
layout: default
title: MQWeb &bull; SubController
api: true
---
SubController
=============

The second part of the URI must be `sub` to call the SubController.

##<a name="inquire"></a>inquire
Get information about one or more subscriptions. This action executes the 
MQCMD_INQUIRE_SUB pcf command.

The returned JSON object will have a `subs` array. When a WebSphere MQ error 
occurred there will be no `subs` array, but instead an `error` object is 
returned.

###<a name="inquireUrl"></a>URL Parameters
`/api/sub/inquire/<QueueManager>/<SubName>`

####<a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

####<a name="inquireUrlSubName"></a>SubName
The unique identifier of the application for a subscription. A generic 
value is allowed. This parameter is optional.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queuemanager is a member of a 
queue-sharing group. This parameter applies to z/OS only.

####<a name="inquireQueryDurable"></a>Durable
Restrict the type of subscriptions by specifying `Yes`, `No` or `All`. The 
value is case sensitive.

####<a name="inquireQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, all subscriptions starting with SYSTEM. will be discarded.
This parameter is optional. By default the value is set to `false`.

####<a name="inquireQueryFilter"></a>Filter
Speficies which filter to use: `I` means Integerfilter, `S` means Stringfilter.
*FilterParam* and *FilterValue* are required to create the filter. When a 
filter can't be build because of too little information, it will be silently 
discarded.

####<a name="inquireQueryFilterOp"></a>FilterOp
{% capture filterop %}{% include filterop.md %}{% endcapture %}
{{ filterop | markdownify }}

####<a name="inquireQueryFilterParam"></a>FilterParam
The name of the parameter to filter on. The names are based on the names used 
in the WebSphere MQ information center.

####<a name="inquireQueryFilterValue"></a>FilterValue
The value to use for filtering. When a string is passed for an Integerfilter, 
a WebSphere MQ constant is assumed.

####<a name="inquireQuerySubAttrs"></a>SubAttrs
With the SubAttrs parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter
are possible. The value must be a valid attribute name.

> Attrs is a synonym for SubAttrs

####<a name="inquireQuerySubId"></a>SubId
Specifies the unique internal subscription identifier. This parameter is 
optional and can't be used together with [SubName](#inquireQuerySubName).

####<a name="inquireQuerySubName"></a>SubName
The unique identifier of the application for a subscription. A generic 
value is allowed. This parameter is optional and can't be used together with
[SubId](#inquireQuerySubId).

This parameter is ignored when there is a URI parameter for a 
[SubName](#inquireUrlSubName).

####<a name="inquireQuerySubType"></a>SubType
Only return the subscriptions of the given type. Possible values are `Admin`,
`API`, `Proxy`, `User` or `All`. Default is `All`. The value is case-sensitive.

###<a name="inquireExample"></a>Example

`/api/sub/inquire/PIGEON/MQWEB.SUB.*`  
`/api/sub/inquire/PIGEON/*`  
`/api/sub/inquire/PIGEON?SubName=*&ExcludeSystem=true`  

This sample is a PHP script that inquires all subscriptions from a queuemanager:

{% include_relative samples/php/sub_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL parameter for
> the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
    {
      'SubName' : 'T*',
      'SubAttrs' : [
        'SubName'
      ]
    }
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ *SubAttrs* is a JSON array with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be *TopicName* for example.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)), 
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all subscriptions:

{% include_relative samples/perl/sub_inq.html %}

