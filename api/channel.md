---
layout: default
title: MQWeb &bull; ChannelController
api: true
---
ChannelController
=================

The second part of the URI must be `channel` to call the ChannelController.
This controller can be used to get information from a channel.

+ [copy](#copy)
+ [create](#create)
+ [inquire](#inquire)
+ [start](#start)
+ [stop](#stop)
+ [remove](#remove)

## <a name="copy"></a>copy
Create a new channel by copying attributes from another channel.
This action will execute MQCMD_COPY_CHANNEL. On success the
JSON response object will have an empty `data` array, on failure an `error`
object.

### <a name="copyUrl"></a>URL Parameters
`api/channel/copy/<QueueManager>/<FromChannelName>/<ToChannelName>`

#### <a name="copyUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="copyUrlFromChannel"></a>FromChannelName
The name of a channel to copy from. This parameter is optional if you use the
corresponding query parameter.

#### <a name="copyUrlToChannelName"></a>ToChannelName
The name of the new channel. This parameter is optional if you use the
corresponding query parameter.

#### <a name="copyUrlChannelType"></a>ChannelType
The type of a channel. ChannelType must be `Sender`, `Server`,
`Receiver`, `Requester`, `Server-connection`, `Client-connection`,
`Cluster-receiver` or `Cluster-sender`. This value is case-sensitive.
This parameter is optional if you use the corresponding query parameter.

### <a name="copyQuery"></a>Query Parameters
All query parameters are copied as attribute for the channel. *FromChannelName*,
*ToChannelName* and *ChannelType* are required. When a [POST](#copyJSON) request
is used, all query parameters will be ignored.

`/api/channel/copy/PIGEON/CL.MQWEB.01/CL.MQWEB.01?ChannelType=ServerConnection`  

### <a name="copyJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object
to create a queue. All URL parameters (except queuemanager) and query parameters
are ignored.

## <a name="create"></a>create
Create a new channel. This action will execute MQCMD_CREATE_CHANNEL. On success
the JSON response object will have an empty `data` array, on failure an `error`
object.

### <a name="createUrl"></a>URL Parameters
`api/queue/create/<QueueManager>/<ChannelName>/<ChannelType>`

#### <a name="createUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="createUrlChannelName"></a>ChannelName
The name of a channel.
This parameter is optional if you use the corresponding query parameter.

#### <a name="createUrlChannelType"></a>ChannelType
The type of a channel. ChannelType must be `Sender`, `Server`,
`Receiver`, `Requester`, `Server-connection`, `Client-connection`,
`Cluster-receiver` or `Cluster-sender`. This value is case-sensitive.
This parameter is optional if you use the corresponding query parameter.

### <a name="createQuery"></a>Query Parameters
All query parameters are copied as attribute for the channel.
*ChannelName* and *ChannelType* are required. If a channelname is passed on
the URL, the ChannelName query parameter will be ignored. If a channeltype is
passed on the URL, the ChannelType query parameter will be ignored.

### <a name="createJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object
to create a channel. All URL parameters (except queuemanager) and query
parameters are ignored.

## <a name="inquire"></a>inquire
Get information about one or more channels. This action executes the
MQCMD_INQUIRE_CHANNEL pcf command. On success, the returned JSON object will
have a `data` array, on failure an `error` object.

### <a name="inquireURL"></a>URL Parameters
`/api/channel/inquire/<QueueManager>/<ChannelName>/<ChannelType>`

#### <a name="inquireURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required!

#### <a name="inquireURLChannelName"></a>ChannelName
The name of a channel. Generic names are supported. When this parameter is used,
query parameters are ignored. This parameter is optional.

#### <a name="inquireURLChannelType"></a>ChannelType
The type of the channel. ChannelType must be `All`, `Sender`, `Server`,
`Receiver`, `Requester`, `Server-connection`, `Client-connection`,
`Cluster-receiver` or `Cluster-sender`. This value is case-sensitive.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryChannelAttrs"></a>ChannelAttrs
With the *ChannelAttrs* parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter are
possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for ChannelAttrs

#### <a name="inquireQueryChannelName"></a>ChannelName
The name of a channel. Generic names are supported. This parameter is ignored
when a [channelname](#inquireURLChannelName) is passed as URL parameter. When
this parameter is omitted, * will be used. `name` is a synonym for this
parameter.

#### <a name="inquireQueryChannelType"></a>ChannelType
ChannelType must be `All`, `Sender`, `Server`, `Receiver`, `Requester`,
`Server-connection`, `Client-connection`, `Cluster-receiver` or
`Cluster-sender`. This value is case-sensitive. When channel type is passed as
[URL parameter](#inquireURLChannelType), this query parameter is ignored.

#### <a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queuemanager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="inquireQueryDefaultChannelDisposition"></a>DefaultChannelDisposition
This parameter is not allowed for *Client-connection* channels and only applies
to z/OS. Possible values are `All`, `Private` or `Shared`. It Specifies the
disposition of the channels for which information is to be returned.

#### <a name="inquiryQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, all channels with a name starting with SYSTEM. will be
discarded. This parameter is optional. By default the value is set to `false`.

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

#### <a name="inquireQueryQSGDisposition"></a>QSGDisposition
Disposition of the object within the group. Possible values are `Live`, `Copy`,
`Group`, `QMgr`, `Private` or `All`. This parameter applies to z/OS only.

### <a name="inquireExample"></a>Example
`/api/channel/inquire/PIGEON`  
`/api/channel/inquire/PIGEON/CL.*`  
`/api/channel/inquire/PIGEON?ChannelName=CL.*`

{% capture sample %}{% include_relative samples/php/channel_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
    {
      'ChannelName' : 'SYSTEM*',
      'ChannelAttrs' : [
        'ChannelName',
        'ConnectionName'
      ]
    }
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ ChannelName is a mandatory property. No default values is used.
+ [ChannelAttrs](#inquireQueryChannelAttrs) is a JSON array with attributenames
  as element.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on
  parameters which has integer values, while *StringFilterCommand* can be used
  to filter on parameters with string values. The filter object has these
  three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)),
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue
  (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all channels starting with `SYSTEM`
and only returns the channelname and connectionname (if available):

{% capture sample %}{% include_relative samples/perl/channel_inq.md %}{% endcapture %}
{{ sample | markdownify }}

## <a name="start"></a>start
Starts a channel. This action will execute MQCMD_START_CHANNEL.
On success the JSON response object will have an empty `data` array,
on failure an `error` object.

### <a name="startUrl"></a>URL Parameters
`api/channel/start/<QueueManager>/<ChannelName>`

#### <a name="startUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="startUrlChannel"></a>ChannelName
The name of a channel to start. This parameter is optional if you use the
corresponding query parameter.

### <a name="startQuery"></a>Query Parameters

#### <a name="startQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queuemanager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="startQueryChannelDisposition"></a>ChannelDisposition
Specifies the disposition of the channels to be started.
Possible values are `Private`, `Shared`, or `FixShared`.
This parameter applies to z/OS only.

## <a name="stop"></a>stop
Stops a channel by. This action will execute MQCMD_STOP_CHANNEL.
On success the JSON response object will have an empty `data` array,
on failure an `error` object.

### <a name="stopUrl"></a>URL Parameters
`api/channel/start/<QueueManager>/<ChannelName>`

#### <a name="stopUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="stopUrlChannel"></a>ChannelName
The name of a channel to start. This parameter is optional if you use the
corresponding query parameter.

### <a name="stopQuery"></a>Query Parameters

#### <a name="stopQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queuemanager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="stopQueryChannelDisposition"></a>ChannelDisposition
Specifies the disposition of the channels to be started.
Possible values are `Private`, `Shared`, or `FixShared`.
This parameter applies to z/OS only.

#### <a name="stopQueryChannelStatus"></a>ChannelStatus
The new state of the channel after the command is executed.
Possible values are `Inactive` or `Stopped`.

#### <a name="stopQueryConnectionName"></a>ConnectionName
This parameter is the connection name of the channel to be stopped.

#### <a name="stopQueryMode"></a>Mode
How the channel must be stopped. Possible values are `Quiesce`, `Force`
or `Terminate`.

#### <a name="stopQMgrName"></a>QMgrName
This parameter is the name of the remote queue manager to which the channel is
 connected.

## <a name="remove"></a>remove
Removes a channel. This action will execute MQCMD_DELETE_CHANNEL. On success the
JSON response object will have an empty `data` array, on failure an `error`
object.

### <a name="removeUrl"></a>URL Parameters
`api/queue/remove/<QueueManager>/<QName>`

#### <a name="removeUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="removeUrlChannelName"></a>ChannelName
The name of the channel to remove. This parameter is optional when the ChannelName
 is passed as query parameter.

### <a name="removeQuery"></a>Query Parameters

#### <a name="removeQueryChannelName"></a>ChannelName
The name of the channel to remove.

#### <a name="removeQueryChannelTable"></a>ChannelTable
Specifies the ownership of the channel definition table that contains the
specified channel definition. Possible values are `QMgr` or `Client-connection`.

#### <a name="removeQueryChannelType"></a>ChannelType
The type of a channel. ChannelType must be `Sender`, `Server`,
`Receiver`, `Requester`, `Server-connection`, `Client-connection`,
`Cluster-receiver` or `Cluster-sender`. This value is case-sensitive.
This parameter is optional if you use the corresponding query parameter.

#### <a name="removeQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="removeQueryQSGDisposition"></a>QSGDisposition
Disposition of the object within the group. Possible values are `Live`, `Copy`,
`Group`, `QMgr`, `Private` or `All`. This parameter applies to z/OS only.
