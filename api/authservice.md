---
layout: default
title: MQWeb &bull; AuthorityServiceController
api: true
---
AuthorityServiceController
==========================

The second part of the URI must be `authservice` to call the 
AuthorityServiceController. This controller can be used to get information 
about the level of function supported by installed authority managers.

##<a name="inquire"></a>inquire
Get information about the level of function supported by installed authority 
managers. This action executes the MQCMD_INQUIRE_AUTH_SERVICE pcf command.

The returned JSON object will have a `authservices` array. When a WebSphere MQ 
error occurred there will be no `authservices` array, but instead an `error` 
object is returned.

###<a name="inquireUrl"></a>URL Parameters
`/api/authservice/inquire/<QueueManager>`

####<a name="inquireURLQueuemanager"></a>QueueManager
The name of the queuemanager. This parameter is required!

###<a name="inquireQuery"></a>Query Parameters

####<a name="inqueryQueryAuthServiceAttrs"></a>AuthServiceAttrs
With the `AuthServiceAttrs` parameter you can specify which attributes must be 
returned from the PCF command. Multiple occurences of this parameter are 
possible. The value must be a (case-sensitive) valid attribute name.

> Attrs is a synonym for AuthServiceAttrs

####<a name="inquireQueryServiceComponent"></a>ServiceComponent
Name of authorization service.

###<a name="inquiryExample"></a>Example
`/api/authservice/inquire/PIGEON`  

{% include_relative samples/php/authservice_inq.html %}

###<a name="inquireJSON"></a>JSON Object
When using an application/json POST request you can post a JSON object with 
names like the query parameters.

> All URL parameters and query parameters are ignored except for the URL 
> parameter for the name of the [queuemanager](#inquireUrlQueueManager).

There are some differences between query parameters and a JSON object:

+ JSON property names are case-sensitive
+ [AuthServiceAttrs](#inqueryQueryAuthServiceAttrs) is a JSON array with 
  attributenames as element.
+ Synonyms can't be used, you need to use the name of the attribute
  as described in the query parameters.
  
{% include_relative samples/perl/authservice_inq.html %}
