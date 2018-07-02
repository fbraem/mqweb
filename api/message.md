---
layout: default
title: MQWeb &bull; MessageController
api: true
---
MessageController
=================

The second part of the URI must be `message` to call the MessageController.

## <a name="browse"></a>browse
The action browse is used to browse one or more messages.
On success, the returned JSON object will have a `data` array, on failure an
`error` object. The MQMD of each message will be translated into a JSON object.
A message id or a correlation id is returned in hexadecimal format.

### <a name="browseURL"></a>URL Parameters
`/api/message/browse/<QueueManager>/<QueueName>/<MessageID>`  

#### <a name="browseURLQueueManager"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="browseURLQueueName"></a>QueueName
The name of a queue. This parameter is required.

#### <a name="browseURLMessageID"></a>messageId
A message id in hex format.

### <a name="browseQuery"></a>Query Parameters

#### <a name="browseLimit"></a>Limit
When no message id is passed, you can limit the number of messages to return.

### <a name="browseExample"></a>Example
`/api/message/browse/PIGEON/MQWEB.TEST.Q1`  
`/api/message/browse/PIGEON/MQWEB.TEST.Q1?Limit=100`

## <a name="dump">
The action dump can be used to browse one message and get the data in
hexidecimal, ebcdic and ascii format.On success, the returned JSON object will
have a `data` array, on failure an  `error` object. The MQMD will be translated
to JSON and results in properties on the `message` object. The message will also have a
 `dump` property which contains some arrays that represents the message data.
The `hex` property will contain an array with a hexadecimal representation of
the message data. The `ascii` property will contain an array with a ASCII
representation of the message data. And the `ebcdic` property will contain an
array with an EBCIDIC representation of the message data.

### <a name="dumpURL"></a>URL Parameters
`/api/message/dump/<QueueManager>/<Queue>/<MessageID>`  

#### <a name="dumpURL"></a>QueueManager
The name of the queuemanager. This parameter is required.

#### <a name="dumpQueueName"></a>QueueName
The name of a queue. This parameter is required.

#### <a name="dumpMessageID"></a>MessageID
A message id in hex format. This parameter is required.

#### <a name="dumpExample"></a>Example
`/api/message/dump/PIGEON/MQWEB.TEST.Q1/414D512053504152524F5720202020202BA4755304000010`  
