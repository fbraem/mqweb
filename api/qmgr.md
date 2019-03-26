---
layout: default
title: MQWeb &bull; QueueManagerController
api: true
---
QueueManagerController
======================

The second part of the URI must be `qmgr` to call the QueueManagerController.
This controller has the following actions:

+ [inquire](#inquire)
+ [ping](#ping)
+ [reset](#reset)

## <a name="inquire"></a>inquire
Returns all attributes of the queuemanager with the given name. This actions
 executes the PCF commando MQCMD_INQUIRE_Q_MGR. On success, the returned JSON
object will have a `data` array, on failure an `error` object.

### <a name="inquireURL"></a>URL Parameters
`/api/qmgr/inquire/<QueueManager>`

#### <a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queue manager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="inquireQueryQMgrAttrs"></a>QMgrAttrs
With the *QMgrAttrs* parameter you can specify which attributes must
be returned from the PCF command. Multiple occurences of this parameter are
possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for QMgrAttrs

### Example
`/api/qmgr/inquire/PIGEON`

{% capture sample %}{% include_relative samples/php/qmgr_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
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

{% capture sample %}{% include_relative samples/perl/qmgr_inq.md %}{% endcapture %}
{{ sample | markdownify }}

## <a name="ping"></a>ping
This action executes the PCF command MQCMD_PING_Q_MGR. On success, the returned JSON
object will have an empty `data` array, on failure an `error` object.

### <a name="pingURL"></a>URL Parameters
`/api/qmgr/ping/<QueueManager>`

#### <a name="pingUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

### Example
`/api/qmgr/ping/PIGEON`

## <a name="reset"></a>reset
This action executes the PCF command MQCMD_RESET_Q_MGR. On success, the returend
JSON object will have an empty `data` array, on failure an `error` object.

### <a name="resetURL"></a>URL Parameters
`/api/qmgr/reset/<QueueManager>/<Action>`

#### <a name="resetUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="resetUrlAction"></a>Action
Specifies the action to take place. Possible values are `AdvanceLog`,
`Statistics` or `PubSub`. This parameter is optional, if you use the query
parameter.

### <a name="resetQuery"></a>Query Parameters

#### <a name="resetQueryAction"></a>Action
Specifies the action to take place. Possible values are `AdvanceLog`,
`Statistics` or `PubSub`. This option is optional when action is passed on the
URL.

#### <a name="resetQueryChildName"></a>ChildName
The name of the child queue manager for which the hierarchical connection is to
be forcibly canceled. This parameter is valid only when the action parameter has
the value `PubSub`.

#### <a name="resetQueryParentName"></a>ParentName
The name of the parent queue manager for which the hierarchical connection is to
be forcibly canceled. This parameter is valid only when the action parameter has
the value `PubSub`.
