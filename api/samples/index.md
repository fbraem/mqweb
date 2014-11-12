---
layout: default
title: MQWeb &bull; Samples
accordion: true
---
Samples
=======
All languages with the capability to use a HTTP client can be used to get
information from WebSphere MQ using the MQWeb HTTP server. On this page you
get an overview of all samples that are available for MQWeb.

[PHP](#PHP)  
[Perl](#Perl)  
[Ruby](#Ruby)  
[JavaScript](#JavaScript)  
[Java](#Java)  

<a name="PHP"></a>PHP
---
With [PHP](http://php.net/) you can use the 
[curl](http://php.net/manual/en/book.curl.php) module, 
[Guzzle](https://github.com/guzzle) or any other 
[http client](https://www.google.be/#q=php+http+client) implementation.

###Authentication Information
{% include accordion_sample.html language="php" id="1" title="Inquire" sample="authinfo_inq" %}

###Authority Records
{% include accordion_sample.html language="php" id="2" title="Inquire" sample="authrec_inq" %}

###Channels
{% include accordion_sample.html language="php" id="3" title="Inquire" sample="channel_inq" %}

###Channel Statuses
{% include accordion_sample.html language="php" id="4" title="Inquire" sample="chstatus_inq" %}

###Listeners
{% include accordion_sample.html language="php" id="5" title="Inquire" sample="listener_inq" %}

###Listener Statuses
{% include accordion_sample.html language="php" id="6" title="Inquire" sample="lsstatus_inq" %}

###Namelists
{% include accordion_sample.html language="php" id="7" title="Inquire" sample="namelist_inq" %}

###Processes
{% include accordion_sample.html language="php" id="8" title="Inquire" sample="process_inq" %}

###QueueManager
{% include accordion_sample.html language="php" id="9" title="Inquire" sample="qmgr_inq" %}

###QueueManager Status
{% include accordion_sample.html language="php" id="10" title="Inquire" sample="qmstatus_inq" %}

###Queues
{% include accordion_sample.html language="php" id="11" title="Inquire" sample="queue_inq" %}
{% include accordion_sample.html language="php" id="12" title="Inquire (with Guzzle)" sample="guzzle" %}

###Services
{% include accordion_sample.html language="php" id="13" title="Inquire" sample="service_inq" %}

<a name="Perl"></a>Perl
----
With [Perl](https://www.perl.org/) you can use
[LWP](http://search.cpan.org/dist/libwww-perl/lib/LWP.pm) together with
[HTTP::Request::Common](http://search.cpan.org/~gaas/HTTP-Message-6.06/lib/HTTP/Request/Common.pm) 
and [JSON](http://search.cpan.org/~makamaka/JSON-2.90/lib/JSON.pm)
or any other [http client](https://www.google.be/#q=perl+http+client) implementation.

###Authentication Information
{% include accordion_sample.html language="perl" id="1" title="Inquire" sample="authinfo_inq" %}

###Authority Records
{% include accordion_sample.html language="perl" id="2" title="Inquire" sample="authrec_inq" %}

###Channels
{% include accordion_sample.html language="perl" id="3" title="Inquire" sample="channel_inq" %}

###Channel Statuses
{% include accordion_sample.html language="perl" id="4" title="Inquire" sample="chstatus_inq" %}

###Listeners
{% include accordion_sample.html language="perl" id="5" title="Inquire" sample="listener_inq" %}

###Listener Statuses
{% include accordion_sample.html language="perl" id="6" title="Inquire" sample="lsstatus_inq" %}

###Namelists
{% include accordion_sample.html language="perl" id="7" title="Inquire" sample="namelist_inq" %}

###Processes
{% include accordion_sample.html language="perl" id="8" title="Inquire" sample="process_inq" %}

###QueueManager
{% include accordion_sample.html language="perl" id="9" title="Inquire" sample="qmgr_inq" %}

###QueueManager Status
{% include accordion_sample.html language="perl" id="10" title="Inquire" sample="qmstatus_inq" %}

###Queues
{% include accordion_sample.html language="perl" id="11" title="Inquire" sample="queue_inq" %}

###Services
{% include accordion_sample.html language="perl" id="12" title="Inquire" sample="service_inq" %}

<a name="Ruby"></a>Ruby
----
The Ruby Standard Library contains [Net/HTTP](http://ruby-doc.org/stdlib/libdoc/net/http/rdoc/)
that implements an HTTP client API for Ruby. Or you can use one of the
[HTTP clients](https://www.ruby-toolbox.com/categories/http_clients)
 from the [Ruby Toolbox](https://www.ruby-toolbox.com).

###Queues
{% include accordion_sample.html language="ruby" id="1" title="Inquire" sample="queue_inq" %}

<a name="JavaScript"></a>JavaScript
----------
JavaScript can be used on the client side and on server side. In a browser
you can use [AJAJ](http://en.wikipedia.org/wiki/AJAJ) to call the MQWeb API's, 
on the server you can use [nodejs](http://nodejs.org/) to run your scripts.

{% include accordion_sample.html language="js" id="1" title="Inquire" sample="queue_inq" %}

<a name="Python"></a>Python
------

<a name="Java"></a>Java
----


