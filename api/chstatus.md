---
layout: default
title: MQWeb &bull; ChannelStatusController
api: true
---
ChannelStatusController
=======================

The second part of the URI must be `chstatus` to call the ChannelStatusController.
This controller can be used to get status information from channels.

## <a name="inquire"></a>inquire
Returns all status attributes of channels which matches the given name.
This action executes the MQCMD_INQUIRE_CHANNEL_STATUS pcf command.
On success, the returned JSON object will have a `data` array, on failure an
 `error` object.

### <a name="inquireURL"></a>URL Parameters
`/api/chstatus/inquire/<QueueManager>/<ChannelName>`

#### <a name="inquireURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required!

#### <a name="inquireURLChannelName"></a>ChannelName
The name of a channel. When this parameter is used,
the [ChannelName](#inquireQueryChannelName) parameter is ignored.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryChannelName"></a>ChannelName
The name of a channel. When this query parameter is not available, a *
will be used which matches all channels. `name` can be used as synonym. This
parameter will be ignored when a channelname is passed as URL parameter.

#### <a name="inquireQueryChannelInstanceType"></a>ChannelInstanceType
`Current Channel`, `Saved Channel` or `Short Channel` are the possible values.
The default is `Current Channel`. The value is case sensitive.

#### <a name="inquireQueryChannelType"></a>ChannelType
Possible values are `All`, `Sender`, `Server`, `Receiver`,
`Requester`, `Server-connection`, `Client-connection`, `Cluster-receiver`
and `Cluster-sender`. The value is case-sensitive. This is actually a shortcut
for a [Filter](#inquireQueryFilter) :

    filter=I&filter_param=ChannelType&filter_op=EQ&filter_value=<value>

It's a numeric filter, because ChannelType is an integer constant. When a string
value is passed, MQWeb will try to convert the string to the corresponding
constant.

> Be aware that only one integer/string filter can be used for each call.

#### <a name="inquireQueryClientIdentifier"></a>ClientIdentifier
The ClientId of the client.

#### <a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="inquireQueryConnectionName"></a>ConnectionName
If this parameter is present, eligible channel instances are limited to those
using this connection name.

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

#### <a name="inquireQueryXmitQName"></a>XmitQName
If this parameter is present, eligible channel instances are limited to those
using this transmission queue.

### <a name="inquire"></a>Example
`/api/chstatus/inquire/PIGEON`  
`/api/chstatus/inquire/PIGEON/CL.MQWEB`  
`/api/chstatus/inquire/PIGEON?Name=CL.*&ChannelType=Server-connection`  

{% capture sample %}{% include_relative samples/php/chstatus_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ ChannelName is a mandatory property. No default values is used.
+ [ChannelInstanceAttrs](#inquireQueryChannelInstanceAttrs) is a JSON array
  with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be
  [ChannelName](#inquireQueryChannelName) for example.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on
  parameters which has integer values, while *StringFilterCommand* can be used
  to filter on parameters with string values. The filter object has these
  three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)),
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue
  (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all channel statuses for
Server-connection channels.

{% capture sample %}{% include_relative samples/perl/chstatus_inq.md %}{% endcapture %}
{{ sample | markdownify }}
