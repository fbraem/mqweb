---
layout: default
title: MQWeb &bull; SubStatusController
api: true
---
SubStatusController
===================

The second part of the URI must be `sbstatus` to call the SubStatusController.

## <a name="inquire"></a>inquire
Get status information about one or more subscriptions. This action executes the
MQCMD_INQUIRE_SUB_STATUS pcf command.  On success, the returned JSON 
object will have a `data` array, on failure an `error` object.

### <a name="inquireUrl"></a>URL Parameters
`/api/sbstatus/inquire/<QueueManager>/<SubName>`

#### <a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="inquireUrlSubName"></a>SubName
The unique identifier of the application for a subscription. A generic
value is allowed. This parameter is optional.

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

#### <a name="inquireQueryStatusAttrs"></a>StatAttrs
With the StatusAttrs parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter
are possible. The value must be a valid attribute name.

> Attrs is a synonym for SubAttrs

#### <a name="inquireQuerySubId"></a>SubId
Specifies the unique internal subscription identifier. This parameter is
optional and can't be used together with [SubName](#inquireQuerySubName).

#### <a name="inquireQuerySubName"></a>SubName
The unique identifier of the application for a subscription. A generic
value is allowed. This parameter is optional and can't be used together with
[SubId](#inquireQuerySubId).

This parameter is ignored when there is a URI parameter for a
[SubName](#inquireUrlSubName).

#### <a name="inquireQuerySubcriptionType"></a>SubcriptionType
Only return the subscriptions of the given type. Possible values are `Admin`,
`API`, `Proxy`, `User` or `All`. Default is `All`. The value is case-sensitive.

### <a name="inquireExample"></a>Example

`/api/sbstatus/inquire/PIGEON/`  
`/api/sbstatus/inquire/PIGEON/*`  

This sample is a PHP script that inquires all statuses from a
 queuemanager:

{% capture sample %}{% include_relative samples/php/substatus_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
    {
      'SubName' : '*'
    }
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ *StatAttrs* is a JSON array with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)),
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This sample is a Perl script that inquires all statuses:

{% capture sample %}{% include_relative samples/perl/substatus_inq.md %}{% endcapture %}
{{ sample | markdownify }}
