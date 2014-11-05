---
layout: default
title: MQWeb &bull; ChannelController
api: true
---
ChannelController
=================

The second part of the URI must be `channel` to call the ChannelController.
This controller can be used to get information from a channel.

##<a name="inquire"></a>inquire
Get information about one or more queues. This action executes the 
MQCMD_INQUIRE_CHANNEL pcf command.

The returned JSON object will have a `channels` array. When a WebSphere MQ 
error occurred there will be no `channels` array, but instead an `error` 
object is returned.

###<a name="inquireURL"></a>URL Parameters
`/api/channel/inquire/<QueueManager>/<ChannelName>/<ChannelType>`

####<a name="inquireURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required!

####<a name="inquireURLChannelName"></a>ChannelName
The name of a channel. Generic names are supported. When this parameter is used, 
query parameters are ignored. This parameter is optional.

####<a name="inquireURLChannelType"></a>ChannelType
The type of the channel. ChannelType must be `All`, `Sender`, `Server`, 
`Receiver`, `Requester`, `Server-connection`, `Client-connection`, 
`Cluster-receiver` or `Cluster-sender`. This value is case-sensitive.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryChannelAttrs"></a>ChannelAttrs
With the *ChannelAttrs* parameter you can specify which attributes must be 
returned from the PCF command. Multiple occurences of this parameter are 
possible. The value must be a (case-sensitive) valid attribute name.

####<a name="inquireQueryChannelName"></a>ChannelName
The name of a channel. Generic names are supported. This parameter is ignored 
when a [channelname](#inquireURLChannelName) is passed as URL parameter. When 
this parameter is omitted, * will be used. `name` is a synonym for this 
parameter.

####<a name="inquireQueryChannelType"></a>ChannelType
ChannelType must be `All`, `Sender`, `Server`, `Receiver`, `Requester`, 
`Server-connection`, `Client-connection`, `Cluster-receiver` or
`Cluster-sender`. This value is case-sensitive. When channel type is passed as 
[URL parameter](#inquireURLChannelType), this query parameter is ignored.

####<a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a 
queue-sharing group. This parameter applies to z/OS only.

####<a name="inquireQueryDefaultChannelDisposition"></a>DefaultChannelDisposition
This parameter is not allowed for *Client-connection* channels and only applies
to z/OS. Possible values are `All`, `Private` or `Shared`. It Specifies the 
disposition of the channels for which information is to be returned.

####<a name="inquiryQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, all channels with a name starting with SYSTEM. will be 
discarded. This parameter is optional. By default the value is set to `false`.

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

####<a name="inquireQueryQSGDisposition"></a>QSGDisposition
Disposition of the object within the group. Possible values are `Live`, `Copy`, 
`Group`, `QMgr`, `Private` or `All`. This parameter applies to z/OS only.

###<a name="inquireExample"></a>Example
`/api/channel/inquire/PIGEON`  
`/api/channel/inquire/PIGEON/CL.*`  
`/api/channel/inquire/PIGEON?ChannelName=CL.*`

{% include_relative samples/php/channel_inq.html %}

###<a name="inquireJSON"></a>JSON Object
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
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be 
  [ChannelName](#inquireQueryChannelName) for example.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on 
  parameters which has integer values, while *StringFilterCommand* can be used 
  to filter on parameters with string values. The filter object has these 
  three properties: Parameter ([FilterParam](#inquireQueryFilterParam)), 
  Operator ([FilterOp](#inquireQueryFilterOp)) and FilterValue 
  ([FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all channels starting with `SYSTEM`
and only returns the channelname and connectionname (if available):

{% include_relative samples/perl/channel_inq.html %}

