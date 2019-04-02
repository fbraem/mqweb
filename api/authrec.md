---
layout: default
title: MQWeb &bull; AuthorityRecordController
api: true
---
AuthorityRecordController
=========================

The second part of the URI must be `authrec` to call the AuthorityRecordController.
This controller can be used to get attributes of authority records.

> AuthorityRecordController is not available for z/OS.

## <a name="inquire"></a>inquire
Get information about one or more authority records. This action executes
the MQCMD_INQUIRE_AUTH_RECS pcf command. On success, the returned JSON object
will have a `data` array, on failure an `error` object.

### <a name="inquireUrl"></a>URL Parameters
`/api/authrec/inquire/<QueueManager>/<ProfileName>`

#### <a name="inquireURLQueuemanager"></a>QueueManager
The name of the queuemanager. This parameter is required!

#### <a name="inquireURLProfileName"></a>ProfileName
This parameter is the name of the profile for which to retrieve authorizations. Generic profile names are supported.
When this parameter is used, the [ProfileName](#inquireQueryProfileName) query parameter is ignored. This parameter
is optional.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryEntityName"></a>EntityName
Depending on the value of [EntityType](#inquireQueryEntityType) this value means: a principal name or a group name.

#### <a name="inquireQueryEntityType"></a>EntityType
Type of the entity: `Group` or `Principal`. The value is case-sensitive.

#### <a name="inquireQueryObjectType"></a>ObjectType
The type of object referred to by the profile. Possible values are : `All`, `Authentication Information`, `Channel`,
`Client-connection Channel`, `Communication Information`, `Listener`, `Namelist`, `Process`, `Queue`, `Queuemanager`,
`Remote Queuemanager`, `Service` or `Topic`. The value is case-sensitive.

#### <a name="inquiryQueryOptions"></a>Options

Options can be set to control the set of authority records that is returned. Options can be passed multiple times.
Possible values are:

+ `Name All Matching` : Return all profiles the names of which match the specified [ProfileName](#inquireQueryProfileName).
+ `Name Explicit` : Return only those profiles the names of which exactly match the [ProfileName](#inquireQueryProfileName).
+ `Entity Explicit` : Return all profiles the entity fields of which match the specified [EntityName](#inquireQueryEntityName).
+ `Entity Set` : Return profiles with the entity of which matches the specified [EntityName](#inquireQueryEntityName) and the profiles pertaining to any groups in which [EntityName](#inquireQueryEntityName) is a member that contribute to the cumulative authority for the specified entity.
+ `Name As Wildcard` : Interpret [ProfileName](#inquireQueryProfileName) as a filter on the profile name of the authority records.

The values are case sensitive.

#### <a name="inqueryQueryProfileAttrs"></a>ProfileAttrs
With the *ProfileAttrs* parameter you can specify which attributes must be returned from the PCF command. Multiple occurences of this parameter are possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for ProfileAttrs

#### <a name="inquireQueryProfileName"></a>ProfileName
The name of the profile for which to retrieve authorizations. When a [ProfileName](#inquireURLProfileName) is passed as
part of the URL, this query parameter will be ignored. A profilename is not
required when [ObjectType](#inquireQueryObjectType) is `Queuemanager`.

#### <a name="inquireQueryServiceComponent"></a>ServiceComponent
Name of the service component.

### <a name="inquiryExample"></a>Example
`/api/authrec/inquire/PIGEON`  

{% capture sample %}{% include_relative samples/php/authrec_inq.md %}{% endcapture %}
{{ sample | markdownify }}

### <a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with names like the
query parameters.

> All URL parameters and query parameters are ignored except for the URL parameter for
> the name of the [queuemanager](#inquireUrlQueueManager).

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ [ProfileAttrs](#inquireQueryProfileAttrs) is a JSON array with attributenames as element.
+ [Options](#inquireQueryOptions) is a JSON array.

{% capture sample %}{% include_relative samples/perl/authinfo_inq.md %}{% endcapture %}
{{ sample | markdownify }}

## <a name="set"></a>set
Sets the authorizations of a profile, object, or class of objects.
Authorizations can be granted to, or revoked from, any number of principals or
groups. This action executes the MQCMD_SET_AUTH_REC pcf command. On success,
the returned JSON object will have an empty `data` array, on failure an `error`
object.

### <a name="setUrl"></a>URL Parameters
`/api/authrec/set/<QueueManager>/<ProfileName>`

#### <a name="setURLQueuemanager"></a>QueueManager
The name of the queuemanager. This parameter is required!

#### <a name="setURLProfileName"></a>ProfileName
This parameter is the name of the profile for which to set authorizations.
Generic profile names are supported.  If you specify an explicit profile name,
the object must exist. When this parameter is used, the
[ProfileName](#setQueryProfileName) query parameter is ignored. Passing a
profilename is required.

### <a name="setQuery"></a>Query Parameters

#### <a name="setQueryAuthorityAdd"> AuthorityAdd
The authority values to set. Repeat the parameter for multiple values. Possible
values are `Alt User Authority`, `Browse`, `Clear`, `Connect`, `Create`,
`Delete`, `Display`, `Input`, `Inquire`, `Output`, `Pass All Context`,
`Pass Identity Context`, `Set`, `Set All Context`, `Set Identity Context`,
`Control`, `Control Extended`, `Publish`, `Subscribe`, `Resume`, `System`,
`All`, `All Admin` and `All MQI`.

#### <a name="setQueryAuthorityRemove"> AuthorityRemove
The authority values to remove. Repeat the parameter for multiple values. See
[AuthorityAdd](#setQueryAuthorityAdd) for possible values.

#### <a name="setQueryGroupNames"> GroupNames
The names of groups having their authorizations set or removed. Repeat the
parameter for multiple values.

#### <a name="setQueryObjectType"></a>ObjectType
The type of object for which to set authorizations. Possible values are:
`Authentication Information`, `Channel`, `Client-connection Channel`,
`Communication Information`, `Listener`, `Namelist`, `Process`, `Queue`,
`Queuemanager`, `Remote Queuemanager`, `Service` or `Topic`. This parameter is
required.

#### <a name="setQueryPrincipalNames"> PrincipalNames
The names of principals having their authorizations set or removed. Repeat the
parameter for multiple values.

#### <a name="setQueryProfileName"></a>ProfileName
The name of the profile for which to set authorizations. When a
[ProfileName](#setURLProfileName) is passed as part of the URL, this query
parameter will be ignored.

#### <a name="setQueryServiceComponent"> ServiceComponent
If installable authorization services are supported, this parameter specifies
the name of the authorization service to which the authorizations apply.

### <a name="setExample"></a>Example
`/api/authrec/set/PIGEON/MQWEB.TEST.Q1?AuthorityAdd=Browse&ObjectType=Queue&GroupNames=mqweb`
`/api/authrec/set/PIGEON/MQWEB.TEST.Q1?AuthorityRemove=Browse&ObjectType=Queue&GroupNames=mqweb`

### <a name="setJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with
names like the query parameters.

{% highlight javascript %}
    {
      "ProfileName" : "MQWEB.TEST.Q1",
      "ObjectType" : "Queue",
      "AuthorityAdd" : [
        "Browse", "Input"
      ],
      "GroupNames" : [
        "mqweb"
      ]
    }
{% endhighlight %}

There are some differences between query parameters and a JSON object:

+ *AuthorityAdd*, *AuthorityRemove*, *GroupNames* and *PrincipalNames* are
arrays.
