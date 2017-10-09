---
layout: default
title: MQWeb &bull; QueueController
api: true
---
QueueController
===============

The second part of the URI must be `queue` to call the QueueController.
This controller has the following actions:

+ [create](#create)
+ [inquire](#inquire)

## <a name="create"></a>create
Create a new queue. This action will execute MQCMD_CREATE_Q. On success the
JSON response object will have an empty `data` array, on failure an `error`
object.

### <a name="createUrl"></a>URL Parameters
`api/queue/create/<QueueManager>/<QName>`

#### <a name="createUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="createUrlQName"></a>QName
The name of a queue. This parameter is optional.

### <a name="createQuery">Query Parameters
All query parameters are copied as attribute for the queue. *QName* and *QType*
are required. If a queuename is passed on the URL, the QName query parameter
will be ignored.

### <a name="createJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object
to create a queue. All URL parameters (except queuemanager) and query parameters
are ignored.

## <a name="inquire"></a>inquire
Get information about one or more queues. This action executes the
 MQCMD_INQUIRE_Q pcf command. On success, the returned JSON
object will have a `data` array, on failure an `error` object.

### <a name="inquireUrl"></a>URL Parameters
`/api/queue/inquire/<QueueManager>/<QName>`

#### <a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="inquireUrlQName"></a>QName
The name of a queue. Generic queue names are supported.
This parameter is optional.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryCFStructure"></a>CFStructure
Specifies the name of the storage class. This parameter is valid only on z/OS.

#### <a name="inquireQueryClusterInfo"></a>ClusterInfo
When `true` cluster information about queues and other queues in the repository
that match the selection criteria is returned. By default this is `false`.

#### <a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="inquireQueryCurrentQDepth"></a>CurrentQDepth
Only return queues which have at least *CurrentQDepth* messages. This is
actually a shortcut for a [Filter](#inquireQueryFilter) :

    filter=I&filter_param=CurrentQDepth&filter_op=NLT&filter_value=<value>

> Be aware that only one integer/string filter can be used for each call.

#### <a name="inquireQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, all queues starting with SYSTEM. will be discarded.
This parameter is optional. By default the value is set to `false`.

#### <a name="inquireQueryExcludeTemp"></a>ExcludeTemp
When value is `true`, all temporary queues will be discarded.
This parameter is optional. By default the value is set to `false`.

> When the [QAttrs](#inquireQueryQAttrs) parameter is used, the DefinitionType
> attribute must be selected to make this work.

#### <a name="inquireQueryFilter"></a>Filter
Speficies which filter to use: `I` means Integerfilter, `S` means Stringfilter.
*FilterParam* and *FilterValue* are required to create the filter. When a filter can't be build
because of too little information, it will be silently discarded.

#### <a name="inquireQueryFilterOp"></a>FilterOp
{% capture filterop %}{% include filterop.md %}{% endcapture %}
{{ filterop | markdownify }}

#### <a name="inquireQueryFilterParam"></a>FilterParam
The name of the parameter to filter on. The names are based on the names used in the WebSphere MQ information center.

#### <a name="inquireQueryFilterValue"></a>FilterValue
The value to use for filtering. When a string is passed for an Integerfilter, a WebSphere MQ constant is assumed.

#### <a name="inquireQueryPageSetID"></a>PageSetID
This parameter specifies that eligible queues are limited to those having the
specified *PageSetID* value. This parameter applies to z/OS only.

#### <a name="inquireQueryQAttrs"></a>QAttrs
With the QAttrs parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter
are possible. The value must be a valid attribute name.

> Attrs is a synonym for QAttrs

#### <a name="inquireQueryQName"></a>QName
Only return queues with a name that matches *QName*. By
default * is used which matches all queues. This parameter is ignored when
there is a URI parameter for a queuename.

#### <a name="inquireQueryQSGDisposition"></a>QSGDisposition
Disposition of the object within the group. Possible values are `Live`, `Copy`,
`Group`, `QMgr`, `Private` or `All`. This parameter applies to z/OS only.

#### <a name="inquireQueryQType"></a>QType
Only return the queues of the given type. Possible values are `Local`,
`Remote`, `Model`, `Alias`, `Cluster` or `All`. Default is `All`. The
value is case-sensitive.

For compatibility reasons with older versions this parameter can also
be passed as *QueueType*.

#### <a name="inquireQueryUsage"></a>Usage
Only return queues with the given usage type. Use `normal` or `xmitq`. When
this parameter is not set, all queues will be returned. The value is not
case-sensitive.

> This parameter doesn't actually exist for MQCMD_INQUIRE_Q, but because it
> is useful to only get information for transmission queues this
> parameter is added.

> When the QAttrs parameter is used, the Usage attribute must be selected to
> make this work.

### <a name="inquireExample"></a>Example

`/api/queue/inquire/PIGEON/MQWEB.TEST.Q1`  
`/api/queue/inquire/PIGEON/*`  
`/api/queue/inquire/PIGEON?QName=*&CurrentQDepth=1`  
`/api/queue/inquire/PIGEON?QAttrs=QName&QAttrs=CurrentQDepth`

This sample is a PHP script that inquires all SYSTEM queues from the PIGEON
queuemanager:

{% capture sample %}{% include_relative samples/php/queue_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
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
+ *CurrentQDepth* can't be used. You need to use an *IntegerFilterCommand* to do the same.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)),
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all SYSTEM queues and shows the
current depth of these queues:

{% capture sample %}{% include_relative samples/perl/queue_inq.md %}{% endcapture %}
{{ sample | markdownify }}
