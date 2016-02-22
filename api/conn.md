---
layout: default
title: MQWeb &bull; ConnectionController
api: true
---
ConnectionController
====================

The second part of the URI must be `conn` to call the ConnectionController.

## <a name="inquire"></a>inquire
Get information from clusters. This action executes the
MQCMD_INQUIRE_CONNECTION pcf command.

The returned JSON object will have a `connections` array. When a WebSphere MQ
error occurred, there will be no `connections` array, but instead an `error`
object is returned.

### <a name="inquireURL"></a>URL Parameters
`/api/conn/inquire/<QueueManager>/<ConnectionId>`

#### <a name="inquireURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required!

#### <a name="inquireURLConnectionId"></a>ConnectionId
A hexadecimal representation of a connection id. When this parameter is used,
the query parameter for [ConnectionId](#inquireQueryConnectionId) is ignored.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryConnectionAttrs"></a>ConnectionAttrs
With the *ConnectionAttrs* parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter are
possible. The value must be a (case-sensitive) valid attribute name. *attrs* is
a synonym for this parameter.

#### <a name="inquireQueryConnectionId"></a>ConnectionId
A hexadecimal representation of a connection id. This parameter is ignored,
when a [URL parameter](#inquireURLConnectionId) is used.

#### <a name="inquireQueryConnInfoType"></a>ConnInfoType
Type of connection information to be returned. Possible values are `Connection`,
`Handle` or `All`. Values are case-sensitive. Default is `Connection`.

#### <a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a
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

#### <a name="inquireQueryURDisposition"></a>URDisposition
The unit of recovery disposition associated with a connection (only valid
on z/OS). Possible values are `All`, `Group` or `Queuemanager`. Values
are case-sensitive.

### <a name="inquireExample"></a>Example
`/api/conn/inquire/PIGEON`  

{% capture sample %}{% include_relative samples/php/conn_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL
> parameter for the name of the [queuemanager](#inquireURLQueueManager).

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive.
+ [ConnectionAttrs](#inquireQueryConnectionAttrs) is a JSON array with
  attributenames as element.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on
  parameters which has integer values, while *StringFilterCommand* can be used
  to filter on parameters with string values. The filter object has these
  three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)),
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue
  (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

{% capture sample %}{% include_relative samples/perl/conn_inq.md %}{% endcapture %}
{{ sample | markdownify }}
