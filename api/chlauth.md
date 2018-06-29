---
layout: default
title: MQWeb &bull; ChannelAuthenticationController
api: true
---
ChannelAuthenticationController
===============================
The second part of the URI must be `chlauth` to call the
 ChannelAuthenticationController.

> Because IBM is not consistent in naming the attributes in the MQ Information
> Center it is possible that some attributes/parameters doesn't have the name
> as defined in the MQ Information Center!

## <a name="inquire"></a>inquire
Get info about the channel authenthication records. This api executes the MQCMD_INQUIRE_CHLAUTH_RECS
pcf command.
On success, the returned JSON object will have a `data` array, on failure an
 `error` object.

### <a name="inquireUrl"></a>URL Parameters
`/api/chlauth/inquire/<QueueManager>/<ChannelName>`  
`/api/chlauth/inquire/<QueueManager>/<ChannelName>?Match=RunCheck&ClientUserIdentifier=<UserId>&ConnectionName=<Ip>`

#### <a name="inquireUrlQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="inquireUrlChannelName"></a>ChannelName.
A generic channelname. When omitted, an asterisk (\*) as wildcard will be used.

### <a name="inquireQuery"></a>Query Parameters

#### <a name="inquireQueryChannelAuthAttrs"></a>ChannelAuthAttrs
With the ChannelAuthAttrs parameter you can specify which attributes must be
returned from the PCF command. Multiple occurences of this parameter
are possible. The value must be a valid attribute name.

> Attrs is a synonym for ChannelAuthAttrs

#### <a name="inquireQueryCheckClient"></a>CheckClient
The user ID and password requirements for the client connection to be successful. The following values are valid: *RequiredAdmin*, *Required*  and *AsQMgr*.

#### <a name="inquireQueryClientUserIdentifier"></a>ClientUserIdentifier
The client asserted user ID to be mapped to a new user ID.
This parameter is only valid with [Type](#inquireQueryType) is *UserMap* and
[Match](#inquireQueryMath) is *RunCheck*.

> The documentation in the MQ Information Center uses the name *ClntUser* for this parameter.
> Because internally the MQCACH_CLIENT_USER_ID constant is used, MQWeb uses the name *ClientUserIdentifier*.

#### <a name="inquireQueryCommandScope"></a>CommandScope
Specifies how the command is executed when the queuemanager is a member of a
queue-sharing group. This parameter applies to z/OS only.

#### <a name="inquireQueryConnectionName"></a>ConnectionName
The IP address to be mapped. This is only valid when [Match](#inquireQueryMath) is *RunCheck*.
> The documentation in the MQ Information Center uses the name *Address* for this parameter.
> Because internally the MQCACH_CONNECTION_NAME constant is used, MQWeb uses the name *ConnectionName*.

#### <a name="inquireQueryFilter"></a>Filter
Speficies which filter to use: `I` means Integerfilter, `S` means Stringfilter.
*FilterParam* and *FilterValue* are required to create the filter. When a filter can't be build
because of too little information, it will be silently discarded.

#### <a name="inquireQueryFilterOp"></a>FilterOp
{% capture filterop %}{% include filterop.md %}{% endcapture %}
{{ filterop | markdownify }}

#### <a name="inquireQueryFilterParam"></a>FilterParam
The name of the parameter to filter on. The names are based on the names used in the WebSphere MQ information center.

#### <a name="inquireQueryFilterValue"></a>FilterValue
The value to use for filtering. When a string is passed for an Integerfilter, a WebSphere MQ constant is assumed.

#### <a name="inquireQueryMatch"></a>Match
Indicates the type of matching to be applied. Possible values are *RunCheck*,
*Exact*, *Generic* and *All*.

#### <a name="inquireQuerySSLCertIssuer"></a>SSLCertIssuer
Restricts matches to being within certificates issued by a particular Certificate Authority.
This is only valid when [Match](#inquireQueryMath) is *RunCheck*.

#### <a name="inquireQuerySSLPeer"></a>SSLPeer
The distinguished name of the certificate to be matched.
This is only valid when [Match](#inquireQueryMath) is *RunCheck*.

#### <a name="inquireQueryRemoteQmgrName"></a>RemoteQmgrName
The name of the remote partner queue manager to be matched.
This is only valid when [Match](#inquireQueryMath) is *RunCheck*.

> The documentation in the MQ Information Center uses the name *QMName* for this parameter.
> Because internally the MQCA_REMOTE_Q_MGR_NAME constant is used, MQWeb uses the name *RemoteQmgrName*.

#### <a name="inquireQueryType"></a>Type
The type of channel authentication record for which to set allowed partner details or mappings to MCAUSER. Possible values are *BlockUser*, *BlockAddress*, *SSLPeerMap*, *AddressMap*, *UserMap* or *QmgrMap*.

### <a name="inquireExample"></a>Example

`/api/chlauth/inquire/PIGEON`  

### <a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ *ChannelAuthAttrs* is a JSON array with attributenames as element. *Attrs* can't be used as synonym.

This sample is a Python script that inquires the channel authentication records:

{% capture sample %}{% include_relative samples/python/chlauth_inq.md %}{% endcapture %}
{{ sample | markdownify }}

## <a name="set"></a>set
Sets the allowed partner details and mappings to MCAUSER for a channel or set
of channels. This api executes the MQCMD_SET_CHLAUTH_REC pcf command.
