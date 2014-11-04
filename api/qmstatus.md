---
layout: default
title: MQWeb &bull; QueueManagerStatusController
api: true
hideSidebar: true
---
QueueManagerStatusController
============================

The second part of the URI must be `qmstatus` to call the 
QueueManagerStatusController.

##<a name="inquire"></a>inquire
Returns all status information attributes of the queuemanager with the given 
name. The data will be returned in the `status` property of the JSON object. 
When a WebSphere MQ error occurred an `error` property is set and the `status` 
property is not set.

> HTTP_NOT_IMPLEMENTED will be returned when this command is send to a
> queuemanager on a z/OS system. MQCMD_INQUIRE_Q_MGR_STATUS is not available for
> z/OS.

###<a name="inquireURL"></a>URL Parameters
`/api/qmstatus/inquire/<QueueManager>`  

####<a name="inquireURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryQMStatusAttrs"></a>QMStatusAttrs
With the *QMStatusAttrs* parameter you can specify which attributes must 
be returned from the PCF command. Multiple occurences of this parameter are 
possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for QMStatusAttrs

###Example
`/api/qmstatus/inquire/PIGEON`

{% include_relative samples/php/qmstatus_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
{
  'QMStatusAttrs' : [
    'StartDate',
    'StartTime'
  ]
}
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ [QMgrAttrs](#inquireQueryQMStatusAttrs) is a JSON array with attributenames as 
  element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *attrs*, it must be 
  [QMStatusAttrs](#inquireQueryQMStatusAttrs) for example.

This is a Perl example that inquires a queuemanager to get the start date and
time of the queuemanager.

{% include_relative samples/perl/qmstatus_inq.html %}

