---
layout: default
title: MQWeb &bull; QueueManagerStatusController
api: true
---
QueueManagerStatusController
============================

The second part of the URI must be `qmstatus` to call the
QueueManagerStatusController.

## <a name="inquire"></a>inquire
Returns all status information attributes of the queuemanager with the given
name. This action executes the MQCMD_INQUIRE_Q_MGR_STATUS pcf command. On 
success, the returned JSON object will have a `data` array, on failure an
`error` object.

> HTTP_NOT_IMPLEMENTED will be returned when this command is send to a
> queuemanager on a z/OS system. MQCMD_INQUIRE_Q_MGR_STATUS is not available for
> z/OS.

### <a name="inquireURL"></a>URL Parameters
`/api/qmstatus/inquire/<QueueManager>`  

#### <a name="inquireURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryQMStatusAttrs"></a>QMStatusAttrs
With the *QMStatusAttrs* parameter you can specify which attributes must
be returned from the PCF command. Multiple occurences of this parameter are
possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for QMStatusAttrs

### Example
`/api/qmstatus/inquire/PIGEON`

{% capture sample %}{% include_relative samples/php/qmstatus_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
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

{% capture sample %}{% include_relative samples/perl/qmstatus_inq.md %}{% endcapture %}
{{ sample | markdownify }}
