---
layout: default
title: MQWeb &bull; AuthenticationInformationController
api: true
hideSidebar: true
---
AuthenticationInformationController
===================================

The second part of the URI must be `authinfo` to call the AuthenticationInformationController.
This controller can be used to get attributes of authentication information objects.

##inquire

###URL Parameters

`/api/authinfo/inquire/<QueueManager>/<AuthInfoName>`

**QueueManager**

The name of the queuemanager. This parameter is required!

**AuthInfoName** (optional)
  
The name of an authentication information object. When this parameter is used, the AuthInfoName query parameter is ignored.

###Query Parameters

**AuthInfoName** (optional)

The name of an authentication information object. When an authentication information object name is passed as
part of the URL, this query parameter will be ignored. *name* is a synonym for this parameter. When no
*AuthInfoName* parameter is passed, * wil be used as default.

####ExcludeSystem

When value is `true`, all authentication information objects starting with SYSTEM. will be discarded.
This parameter is optional. By default the value is set to `false`.

###Example
`/api/authinfo/inquire/PIGEON`  
`/api/authinfo/inquire/PIGEON/SYSTEM*`  
