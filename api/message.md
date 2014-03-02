---
layout: default
title: MQWeb &bull; MessageController
api: true
hideSidebar: true
---
MessageController
=================

The second part of the URI must be `message` to call the MessageController.
This controller can be used to get messages from a queue.

##Actions

###browse

Name
: browse

Type
: JSON

Parameters
: + **queueManager**
 
    The name of the queuemanager

  + **queueName**

    The name of a queue.
    
  + **messageId** (optional)
  
    A message id in hex format.

Query Parameters
: + **limit** (optional)

    When no message id is passed, you can limit the number of messages to 
    return.

  + **teaser** (optional)

    When a message has format MQFMT_STRING, a part of the message can be
    returned.

Example
: `/api/message/browse/PIGEON/MQWEB.TEST.Q1`  
  `/api/message/browse/PIGEON/MQWEB.TEST.Q1?limit=100`

<div style="clear:both;"> </div>

The returned JSON object will have a `mqweb` object and a `messages` array.
The MQMD of each message will be translated into a JSON object. A message Id
or a correlation id is returned in hexadecimal format. When a WebSphere MQ 
error occurred there will be no `messages` array, but instead an `error` object 
is returned.

###dump

Name
: dump

Type
: JSON

Parameters
: + **queueManager**
 
    The name of the queuemanager

  + **queueName**

    The name of a queue.
    
  + **messageId**
  
    A message id in hex format.

Example
: `/api/message/dump/PIGEON/MQWEB.TEST.Q1/<hex>`  

<div style="clear:both;"> </div>

The returned JSON object will have a `mqweb` object and a `message` object.
The MQMD will be translated to JSON and results in properties on the `message`
object. The message will also have a `dump` property which contains some arrays
that represents the message data. The `hex` property will contain an array with 
a hexadecimal representation of the message data. The `ascii` property will 
contain an array with a ASCII representation of the message data. And the 
`ebcdic` property will contain an array with an EBCIDIC representation of the 
message data. 

###event

Name
: event

Type
: JSON

Parameters
: + **queueManager**
 
    The name of the queuemanager

  + **queueName**

    The name of a queue which contains event messages.
    
  + **messageId** (optional)
  
    A message id in hex format. When no message id is set, all event messages
    will be returned.

Query Parameters
: + **limit** (optional)

    When no message id is passed, you can limit the number of messages to 
    return.

Example
: `/api/message/event/PIGEON/SYSTEM.ADMIN.QGMR.EVENT`  

<div style="clear:both;"> </div>

The returned JSON object will have a `mqweb` object and an `events` array. Each
event item, will have a `message` property with a MQMD that is translated to
JSON, it will have an `event` object with the specific event information and a
`reason` object.

