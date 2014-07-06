---
layout: default
title: MQWeb &bull; Connection with SSL
doc_ssl: true
---
MQWeb and SSL connections
=========================

> SSL support in MQWeb is still experimental!

Starting from version 0.0.10 you can use `mqweb.properties` to configure
a connection to a queuemanager which uses a channel with SSL. In prior versions 
this was also possible but you needed to set some environment variables before
starting MQWeb.

> SSL can only be used when MQWeb runs in client mode. `mq.web.mode` must be set 
> to `client`

SSL properties
--------------

The following SSL properties are availabe for use in `mqweb.properties`:

+ mq.web.ssl.keyrepos

    This property specifies the location of the key database file in which keys
    and certificates are stored. The key database file must have the extension
    `kdb`. The value of this property must contain the full path to this file 
    except for the extension. The key database file must have an associated
    password stash file. The password stash file must reside in the same
    directory as the key database file with the same filename but with the
    extension `sth`. This property is required.

+ mq.web.ssl.cipherspec

    The cipher spec used by the channel. This property is required.

+ mq.web.ssl.fips

    Use this field to specify that only FIPS-certified algorithms are used.
    Valid values are `true` or `false`. Default is `false`. This property is
    not required.

+ mq.web.ssl.suiteb

    Specifies whether Suite B compliant cryptography is used and what level of 
    strength is employed. This value is a comma-delimited value with values
    `none`, `128 bit` or `192 bit`. This property is not required.

+ mq.web.ssl.certificate_validation_policy

    Specifies which type of certificate validation policy is used. This can be
    a value of `any` or `rfc5280`. This property is not required.

+ mq.web.ssl.ocsp_url

    The OCSP Responder URL. This property is not required.

When `mq.web.ssl.keyrepos` is set, MQWeb assumes that SSL is always needed to
connect to a queuemanager.

An Example
----------

> In this sample MQWeb will try to connect to a queuemanager named
> SEAGULL using an SSL channel.

### SSL for queuemanager - Part 1

Logon to the server and check if `SSLKEYR` is correctly set on the queuemanager.
For queuemanager SEAGULL, the value would be `/var/mqm/qmgrs/SEAGULL/ssl/key`.

Create a server connection channel:

    DEFINE CHANNEL(CL.SSL) +
    CHLTYPE(SVRCONN) +
    SSLCAUTH(REQUIRED) +
    SSLCIPH(TRIPLE_DES_SHA_US)

> In all samples, the password `ssltest` is used.

When there is no key database file, create it with the following command:

    runmqakm -keydb -create -db "/var/mqm/qmgrs/SEAGULL/ssl/key.kdb" -pw ssltest -type cms -stash

Now you need to create a certificate for the queuemanager. It's important that
the label starts with `ibmwebspheremq` followed by the queuemanager name in 
lowercase. In this example this is `ibmwebspheremqseagull`.

    runmqakm -cert -create -db "/var/mqm/qmgrs/SEAGULL/ssl/key.kdb" -pw ssltest -label ibmwebspheremqseagull -dn "CN=SEAGULL,OU=WMQ,O=ZUMUTA,C=BE"

Now extract the certificate and send the resulting file to the server where
MQWeb runs.

    runmqakm -cert -extract -db "/var/mqm/qmgrs/SEAGULL/ssl/key.kdb" -pw ssltest -label ibmwebspheremqseagull -target "/var/mqm/qmgrs/SEAGULL/ssl/seagull.arm" -format ascii

### SSL for MQWeb - Part 1

Go to the server where MQWeb runs. You also need to create a key database file
here:

    runmqakm -keydb -create -db "/var/mqweb/ssl/key.kdb" -pw ssltest -type cms -stash

Create a certificate for the MQWeb user. It's important that the label starts
with `ibmwebspheremq` and is followed by the name of the user that tries to
connect to the queuemanager.

    runmqakm -cert -create -db "/var/mqweb/ssl/key.kdb" -pw ssltest -label ibmwebspheremqmqweb -dn "CN=SEAGULL,OU=WMQ,O=ZUMUTA,C=BE"

Extract this certificate and send the resulting file to the server where the
queuemanager is running.

    runmqakm -cert -extract -db "/var/mqweb/ssl/key.kdb" -pw ssltest -label ibmwebspheremqmqweb -target "/var/mqweb/ssl/mqweb.arm" -format ascii

Import the certificate of the queuemanager into the key database file

    runmqckm -cert -add -db "/var/mqweb/ssl/key.kdb" -pw ssltest -label ibmwebspheremqseagull -file seagull.arm

### SSL for queuemanager - Part 2

Import the certificate for the MQWeb user into the key database file of the
queuemanager:

    runmqckm -cert -add -db "/var/mqm/qmgrs/SEAGULL/ssl/key.kdb" -pw ssltest -label ibmwebspheremqmqweb -file mqweb.arm

Restart queuemanager SEAGULL or use REFRESH SECURITY TYPE(SSL)

### SSL for MQWeb - Part 2

Check if `mq.web.mode` is set to `client` in `mqweb.properties`. Add the
following properties:

    mq.web.ssl.keyrepos=/var/mqweb/ssl/key
    mq.web.ssl.cipherspec=TRIPLE_DES_SHA_US

and use CL.SSL as channel for queuemanager SEAGULL:

    mq.web.qmgr.SEAGULL.channel=CL.SSL

If all goes well MQWeb can now connect with the SEAGULL using SSL

> If you receive any 2035 (MQRC_NOT_AUTHORIZED) messages, check the security
> settings on the queuemanager.
