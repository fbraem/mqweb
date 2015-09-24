---
layout: default
title: MQWeb &bull; ClusterQueueManagerController
api: true
---
ClusterQueueManagerController
=============================

The second part of the URI must be `clusqmgr` to call the ClusterQueueManagerController.

+ [inquire](#inquire) : Inquire cluster information
+ [suspend](#suspend) : Suspend queuemanager in cluster
+ [resume](#resume) : Resume queuemanager in cluster

##<a name="inquire"></a>inquire
Get information from clusters. This action executes the 
MQCMD_INQUIRE_CLUSTER_Q_MGR pcf command.

The returned JSON object will have a `clusqmgrs` array. When a WebSphere MQ
error occurred, there will be no `clusqmgrs` array, but instead an `error`
object is returned.

###<a name="inquireURL"></a>URL Parameters
`/api/clusqmgr/inquire/<QueueManager>/<ClusterName>/<ClusterQMgrName>`

####<a name="inquireURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required!

####<a name="inquireURLClusterName"></a>ClusterName
The name of a cluster. Generic names are supported. When this parameter is used, 
the query parameter for [ClusterName](#inquireQueryClusterName) is ignored.

####<a name="inquireURLClusterQMgrName"></a>ClusterQMgrName
The name of a queuemanager that is part of the cluster. Generic names are 
supported. When this parameter is used, the query parameter for 
[ClusterQMgr](#inquireQueryClusterQMgrName) is ignored.

###<a name="inquireQuery"></a>Query Parameters

####<a name="inquireQueryChannel"></a>Channel
Specifies that eligible cluster queue managers are limited to those having the 
specified channel name. Generic names are supported.

####<a name="inquireQueryClusterName"></a>ClusterName
The name of a cluster. Generic names are supported. This parameter is ignored, 
when a [URL parameter](#inquireURLClusterName) is used. *name* is a synonym
for this parameter.

####<a name="inquireQueryClusterQMgrAttrs"></a>ClusterQMgrAttrs
With the *ClusterQMgrAttrs* parameter you can specify which attributes must be 
returned from the PCF command. Multiple occurences of this parameter are 
possible. The value must be a (case-sensitive) valid attribute name. *attrs* is
a synonym for this parameter.

####<a name="inquireQueryClusterQMgrName"></a>ClusterQMgrName
The name of a queuemanager that is part of the cluster. This parameter is 
ignored when a [URL parameter](#inquireURLClusterQMgrName) is used. When omitted
a * is used.

####<a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a 
queue-sharing group. This parameter applies to z/OS only.

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
`/api/clusqmgr/inquire/PIGEON/BIRDS`  
`/api/clusqmgr/inquire/PIGEON/BIRDS/BLACKBIRD`  
`/api/clusqmgr/inquire/PIGEON?ClusterName=BIRDS`

{% include_relative samples/php/clusqmgr_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#inquireURLQueueManager).

{% highlight javascript %}
    {
      'ClusterName' : 'BIRDS',
      'ClusterQMgrName' : '*'
    }
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ [ClusterQMgrAttrs](#inquireQueryClusterQMgrAttrs) is a JSON array with 
  attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters. You can't use *name*, it must be 
  [ClusterName](#inquireQueryClusterName) for example.
+ [ClusQMgrName](#inquireQueryClusterQMgrName) is required.
+ A filter is an object: *IntegerFilterCommand* can be used to filter on 
  parameters which has integer values, while *StringFilterCommand* can be used 
  to filter on parameters with string values. The filter object has these 
  three properties: Parameter (see [FilterParam](#inquireQueryFilterParam)), 
  Operator (see [FilterOp](#inquireQueryFilterOp)) and FilterValue 
  (see [FilterValue](#inquireQueryFilterValue)).

> An *IntegerFilterCommand* can't be used together with a *StringFilterCommand*

{% include_relative samples/perl/clusqmgr_inq.html %}

##<a name="suspend">suspend
Suspend the queuemanager in a cluster. This action executes the 
MQCMD_SUSPEND_Q_MGR_CLUSTER pcf command.

The returned JSON object will have an `error` object when a WebSphere MQ
error occurred.

###<a name="suspendURL"></a>URL Parameters
`/api/clusqmgr/suspend/<QueueManager>/<ClusterName>`

####<a name="suspendURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required!

####<a name="suspendURLClusterName"></a>ClusterName
The name of a cluster. When this parameter is used, 
the query parameter for [ClusterName](#suspendQueryClusterName) is ignored.

###<a name="suspendQuery"></a>Query Parameters

####<a name="suspendQueryClusterName"></a>ClusterName
The name of a cluster. This parameter is ignored, when 
a [URL parameter](#suspendURLClusterName) is used.

####<a name="suspendQueryClusterNamelist"></a>ClusterNamelist
The name of a cluster namelist. This parameter can not be used together with
[ClusterName](#suspendURLClusterName).

####<a name="suspendQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a 
queue-sharing group. This parameter applies to z/OS only.

####<a name="suspendQueryMode"></a>Mode
The mode on how the local queue manager is suspended from the cluster. The 
case-sensitive value can be `Quiesce` or `Force`.

###<a name="suspendExample"></a>Example
`/api/clusqmgr/suspend/PIGEON/BIRDS`  
`/api/clusqmgr/suspend/PIGEON?ClusterName=BIRDS&Mode=Quiesce`

{% include_relative samples/php/clusqmgr_suspend.html %}

###<a name="suspendJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#suspendURLQueueManager).

{% highlight javascript %}
    {
      'ClusterName' : 'BIRDS',
      'Mode' : 'Quiesce'
    }
{% endhighlight %}

{% include_relative samples/perl/clusqmgr_suspend.html %}

##<a name="resume">resume
Resume the queuemanager in a cluster. This action executes the 
MQCMD_RESUME_Q_MGR_CLUSTER pcf command.

The returned JSON object will have an `error` object when a WebSphere MQ
error occurred.

####<a name="resumeQueryClusterName"></a>ClusterName
The name of a cluster. This parameter is ignored, when 
a [URL parameter](#resumeURLClusterName) is used.

####<a name="resumeQueryClusterNamelist"></a>ClusterNamelist
The name of a cluster namelist. This parameter can not be used together with
[ClusterName](#resumeURLClusterName).

####<a name="resumeQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a 
queue-sharing group. This parameter applies to z/OS only.

###<a name="resumeExample"></a>Example
`/api/clusqmgr/resume/PIGEON/BIRDS`  
`/api/clusqmgr/resume/PIGEON?ClusterName=BIRDS`

{% include_relative samples/perl/clusqmgr_resume.html %}

###<a name="resumeJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#resumeURLQueueManager).

{% highlight javascript %}
    {
      'ClusterName' : 'BIRDS'
    }
{% endhighlight %}

{% include_relative samples/perl/clusqmgr_resume.html %}

