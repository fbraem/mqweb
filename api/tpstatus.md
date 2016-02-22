---
layout: default
title: MQWeb &bull; TopicStatusController
api: true
---
TopicStatusController
=====================

The second part of the URI must be `tpstatus` to call the TopicStatusController.

## <a name="inquire"></a>inquire
Get status information about one or more topics. This action executes the
MQCMD_INQUIRE_TOPIC_STATUS pcf command.

The returned JSON object will have a `statuses` array. When a WebSphere MQ error
occurred there will be no `statuses` array, but instead an `error` object is
returned.

### <a name="inquireUrl"></a>URL Parameters
`/api/tpstatus/inquire/<QueueManager>/<TopicString>`

#### <a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="inquireUrlTopicName"></a>TopicString
The topic stringname of a topic. This parameter is optional.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queuemanager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="inquireQueryFilter"></a>Filter
Speficies which filter to use: `I` means Integerfilter, `S` means Stringfilter.
*FilterParam* and *FilterValue* are required to create the filter. When a
filter can't be build because of too little information, it will be silently
discarded.

#### <a name="inquireQueryFilterOp"></a>FilterOp
{% capture filterop %}{% include filterop.md %}{% endcapture %}
{{ filterop | markdownify }}

#### <a name="inquireQueryFilterParam"></a>FilterParam
The name of the parameter to filter on. The names are based on the names used
in the WebSphere MQ information center.

#### <a name="inquireQueryFilterValue"></a>FilterValue
The value to use for filtering. When a string is passed for an Integerfilter,
a WebSphere MQ constant is assumed.

#### <a name="inquireQueryStatusType"></a>StatusType
The type of status to return. The value can be `Status`, `Sub` or `Pub` and
is case-sensitive. The default value is `Status`.

#### <a name="inquireQueryQAttrs"></a>TopicStatusAttrs
With the TopicStatusAttrs parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter
are possible. The value must be a valid attribute name.

> Attrs is a synonym for TopicStatusAttrs

#### <a name="inquireQueryTopicName"></a>TopicString
By default * is used which matches all topic strings. This parameter is
ignored when there is a URL parameter for a topicstring.

### <a name="inquireExample"></a>Example

`/api/tpstatus/inquire/PIGEON/`  
`/api/tpstatus/inquire/PIGEON/*`  

This sample is a PHP script that inquires all topic statuses from a
 queuemanager:

{% capture sample %}{% include_relative samples/php/tpstatus_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
    {
      'TopicString' : '*'
    }
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ *TopicStatusAttrs* is a JSON array with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)),
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all topic statuses:

{% capture sample %}{% include_relative samples/perl/tpstatus_inq.md %}{% endcapture %}
{{ sample | markdownify }}
