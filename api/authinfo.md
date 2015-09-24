---
layout: default
title: MQWeb &bull; AuthenticationInformationController
api: true
---
AuthenticationInformationController
===================================

The second part of the URI must be `authinfo` to call the AuthenticationInformationController.
This controller can be used to get attributes of authentication information objects.

##<a name="inquire"></a>inquire
Get information about one or more authentication information objects. 
This action executes the MQCMD_INQUIRE_AUTH_INFO pcf command.

The returned JSON object will have a `authinfos` array. When a WebSphere MQ error occurred 
there will be no `authinfos` array, but instead an `error` object is returned.

###<a name="inquireUrl"></a>URL Parameters
`/api/authinfo/inquire/<QueueManager>/<AuthInfoName>`

####<a name="inquireURLQueuemanager"></a>QueueManager
The name of the queuemanager. This parameter is required!

####<a name="inquireURLAuthInfoName"></a>AuthInfoName
The name of an authentication information object. When this parameter is used, the AuthInfoName query parameter is ignored.
This parameter is optional.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inqueryQueryAuthInfoAttrs"></a>AuthInfoAttrs
With the *AuthInfoAttrs* parameter you can specify which attributes must be returned from the PCF command. Multiple occurences of this parameter are possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for AuthInfoAttrs

####<a name="inquiryQueryAuthInfoName"></a>AuthInfoName
The name of an authentication information object. When an authentication information object name is passed as
part of the URL, this query parameter will be ignored. *name* is a synonym for this parameter. When no
*AuthInfoName* parameter is passed, * wil be used as default.

####<a name="inquiryQueryAuthInfoType"></a>AuthInfoType
Only return authentication information objects of the given type. Possible values are `CRL LDAP`, `OCSP`or `All`.
Default is `All`. The value is case-sensitive.

####<a name="inquiryQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a queue-sharing group.
This parameter applies to z/OS only.

####<a name="inquiryQueryExcludeSystem"></a>ExcludeSystem
When value is `true`, all authentication information objects starting with SYSTEM. will be discarded.
This parameter is optional. By default the value is set to `false`.

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

####<a name="inquiryQueryQSGDisposition"></a>QSGDisposition

Disposition of the object within the group. Possible values are `Live`, `Copy`, `Group`, `QMgr`, `Private` 
or `All`. This parameter applies to z/OS only.

###<a name="inquiryExample"></a>Example
`/api/authinfo/inquire/PIGEON`  
`/api/authinfo/inquire/PIGEON/SYSTEM*`

This sample is a PHP script that inquires all SYSTEM authentication information objects from the PIGEON
queuemanager:

{% include_relative samples/php/authinfo_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with names like the
query parameters.

> All URL parameters and query parameters are ignored except for the URL parameter for
> the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
{
  'AuthInfoName' : 'SYSTEM*',
  'AuthInfoAttrs' : [
    'AuthInfoName'
  ]
}
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ [AuthInfoAttrs](#inquireQueryAuthInfoAttrs) is a JSON array with attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be 
  [AuthInfoName](#inquireQueryAuthInfoName) for example.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on parameters which has
  integer values, while *StringFilterCommand* can be used to filter on parameters with string values.
  The filter object has these three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)), 
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

This is a Perl example that inquires the authentication information objects with
a name starting with SYSTEM.

{% include_relative samples/perl/authinfo_inq.html %}

