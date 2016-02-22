---
layout: default
title: MQWeb &bull; TopicController
api: true
---
TopicController
===============

The second part of the URI must be `topic` to call the TopicController.

## <a name="inquire"></a>inquire
Get information about one or more topics. This action executes the
MQCMD_INQUIRE_TOPIC pcf command.

The returned JSON object will have a `topics` array. When a WebSphere MQ error
occurred there will be no `topics` array, but instead an `error` object is
returned.

### <a name="inquireUrl"></a>URL Parameters
`/api/topic/inquire/<QueueManager>/<TopicName>`

#### <a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="inquireUrlTopicName"></a>TopicName
The name of a topic. Generic topic names are supported.
This parameter is optional.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryClusterInfo"></a>ClusterInfo
When `true` cluster information about topics and other topics in the repository
that match the selection criteria is returned. By default this is `false`.

#### <a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queuemanager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="inquireQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, all topics starting with SYSTEM. will be discarded.
This parameter is optional. By default the value is set to `false`.

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

#### <a name="inquireQueryTopicAttrs"></a>TopicAttrs
With the TopicAttrs parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter
are possible. The value must be a valid attribute name.

> Attrs is a synonym for TopicAttrs

#### <a name="inquireQueryTopicName"></a>TopicName
Only return topics with a name that matches *TopicName*. By
default * is used which matches all topics. *Name* is a synonym for this
parameter.

This parameter is ignored when there is a URI parameter for a topicname.

#### <a name="inquireQueryQSGDisposition"></a>QSGDisposition
Disposition of the object within the group. Possible values are `Live`, `Copy`,
`Group`, `QMgr`, `Private` or `All`. This parameter applies to z/OS only.

#### <a name="inquireQueryTopicType"></a>TopicType
Only return the topics of the given type. Possible values are `Local`,
`Cluster` or `All`. Default is `All`. The value is case-sensitive.

### <a name="inquireExample"></a>Example

`/api/topic/inquire/PIGEON/MQWEB.TOPIC.Q1`  
`/api/topic/inquire/PIGEON/*`  
`/api/topic/inquire/PIGEON?TopicName=*`  
`/api/topic/inquire/PIGEON?TopicAttrs=TopicName`

This sample is a PHP script that inquires all topics from a queuemanager:

{% capture sample %}{% include_relative samples/php/topic_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL parameter for
> the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
    {
      'TopicName' : 'T*',
      'TopicAttrs' : [
        'TopicName'
      ]
    }
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ *TopicAttrs* is a JSON array with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be *TopicName* for example.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)),
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all topics:

{% capture sample %}{% include_relative samples/perl/topic_inq.md %}{% endcapture %}
{{ sample | markdownify }}
