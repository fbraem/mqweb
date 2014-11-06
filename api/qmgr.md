---
layout: default
title: MQWeb &bull; QueueManagerController
api: true
---
QueueManagerController
======================

The second part of the URI must be `qmgr` to call the QueueManagerController.

##<a name="inquire"></a>inquire
Returns all attributes of the queuemanager with the given name. The data will
be returned in the `qmgr` property of the JSON object. When a WebSphere MQ 
error occurred an `error` property is set and the `qmgr` property is not set.

###<a name="inquireURL"></a>URL Parameters
`/api/qmgr/inquire/<QueueManager>`

####<a name="inquireURL"></a>QueueManager
The name of the queuemanager. This parameter is required.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a 
queue-sharing group. This parameter applies to z/OS only.

####<a name="inquireQueryQMgrAttrs"></a>QMgrAttrs
With the *QMgrAttrs* parameter you can specify which attributes must 
be returned from the PCF command. Multiple occurences of this parameter are 
possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for QMgrAttrs

###Example
`/api/qmgr/inquire/PIGEON`

{% include_relative samples/php/qmgr_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

{% highlight javascript %}
{
  'QMgrAttrs' : [
    'QMgrName',
    'QMgrDesc'
  ]
}
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ [QMgrAttrs](#inquireQueryQMgrAttrs) is a JSON array with attributenames as 
  element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *attrs*, it must be 
  [QMgrAttrs](#inquireQueryQMgrAttrs) for example.

This is a Perl example that inquires a queuemanager to get the description of 
the queuemanager.

{% include_relative samples/perl/qmgr_inq.html %}

