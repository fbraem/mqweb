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

##<a name="PHP"></a>PHP
With [PHP](http://php.net/) you can use the 
[curl](http://php.net/manual/en/book.curl.php) module, 
[Guzzle](https://github.com/guzzle) or any other 
[http client](https://www.google.be/#q=php+http+client) implementation.

###Authentication Information
{% include accordion_sample.html language="php" id="authinfo_php_1" title="Inquire" sample="authinfo_inq" %}

###Authority Records
{% include accordion_sample.html language="php" id="authrec_php_1" title="Inquire" sample="authrec_inq" %}

###Channels
{% include accordion_sample.html language="php" id="channel_php_1" title="Inquire" sample="channel_inq" %}

###Channel Statuses
{% include accordion_sample.html language="php" id="chstatus_php_1" title="Inquire" sample="chstatus_inq" %}

###Cluster Queuemanager
{% include accordion_sample.html language="php" id="clusqmgr_php_1" title="Inquire" sample="clusqmgr_inq" %}
{% include accordion_sample.html language="php" id="clusqmgr_php_2" title="Suspend" sample="clusqmgr_suspend" %}
{% include accordion_sample.html language="php" id="clusqmgr_php_3" title="Resume" sample="clusqmgr_resume" %}

###Listeners
{% include accordion_sample.html language="php" id="listener_php_1" title="Inquire" sample="listener_inq" %}

###Listener Statuses
{% include accordion_sample.html language="php" id="lsstatus_php_1" title="Inquire" sample="lsstatus_inq" %}

###Namelists
{% include accordion_sample.html language="php" id="namelist_php_1" title="Inquire" sample="namelist_inq" %}

###Processes
{% include accordion_sample.html language="php" id="process_php_1" title="Inquire" sample="process_inq" %}

###QueueManager
{% include accordion_sample.html language="php" id="qmgr_php_1" title="Inquire" sample="qmgr_inq" %}

###QueueManager Status
{% include accordion_sample.html language="php" id="qmstatus_php_1" title="Inquire" sample="qmstatus_inq" %}

###Queues
{% include accordion_sample.html language="php" id="queue_php_1" title="Inquire" sample="queue_inq" %}
{% include accordion_sample.html language="php" id="queue_php_2" title="Inquire (with Guzzle)" sample="guzzle" %}

###Services
{% include accordion_sample.html language="php" id="service_php_1" title="Inquire" sample="service_inq" %}

###Topics
{% include accordion_sample.html language="php" id="topic_php_1" title="Inquire" sample="topic_inq" %}

##<a name="Perl"></a>Perl
With [Perl](https://www.perl.org/) you can use
[LWP](http://search.cpan.org/dist/libwww-perl/lib/LWP.pm) together with
[HTTP::Request::Common](http://search.cpan.org/~gaas/HTTP-Message-6.06/lib/HTTP/Request/Common.pm) 
and [JSON](http://search.cpan.org/~makamaka/JSON-2.90/lib/JSON.pm)
or any other [http client](https://www.google.be/#q=perl+http+client) implementation.

###Authentication Information
{% include accordion_sample.html language="perl" id="authinfo_perl_1" title="Inquire" sample="authinfo_inq" %}

###Authority Records
{% include accordion_sample.html language="perl" id="authrec_perl_1" title="Inquire" sample="authrec_inq" %}

###Channels
{% include accordion_sample.html language="perl" id="channel_perl_1" title="Inquire" sample="channel_inq" %}

###Channel Statuses
{% include accordion_sample.html language="perl" id="chstatus_perl_1" title="Inquire" sample="chstatus_inq" %}

###Listeners
{% include accordion_sample.html language="perl" id="listener_perl_1" title="Inquire" sample="listener_inq" %}

###Listener Statuses
{% include accordion_sample.html language="perl" id="lsstatus_perl_1" title="Inquire" sample="lsstatus_inq" %}

###Namelists
{% include accordion_sample.html language="perl" id="namelist_perl_1" title="Inquire" sample="namelist_inq" %}

###Processes
{% include accordion_sample.html language="perl" id="process_perl_1" title="Inquire" sample="process_inq" %}

###QueueManager
{% include accordion_sample.html language="perl" id="qmgr_perl_1" title="Inquire" sample="qmgr_inq" %}

###QueueManager Status
{% include accordion_sample.html language="perl" id="qmstatus_perl_1" title="Inquire" sample="qmstatus_inq" %}

###Queues
{% include accordion_sample.html language="perl" id="queue_perl_1" title="Inquire" sample="queue_inq" %}

###Services
{% include accordion_sample.html language="perl" id="service_perl_1" title="Inquire" sample="service_inq" %}

###Topics
{% include accordion_sample.html language="perl" id="topic_perl_1" title="Inquire" sample="topic_inq" %}

##<a name="Ruby"></a>Ruby
The Ruby Standard Library contains [Net/HTTP](http://ruby-doc.org/stdlib/libdoc/net/http/rdoc/)
that implements an HTTP client API for Ruby. Or you can use one of the
[HTTP clients](https://www.ruby-toolbox.com/categories/http_clients)
 from the [Ruby Toolbox](https://www.ruby-toolbox.com).

###Queues
{% include accordion_sample.html language="ruby" id="queue_ruby_1" title="Inquire" sample="queue_inq" %}

##<a name="JavaScript"></a>JavaScript
JavaScript can be used on the client side and on server side. In a browser
you can use [AJAJ](http://en.wikipedia.org/wiki/AJAJ) to call the MQWeb API's, 
on the server you can use [nodejs](http://nodejs.org/) to run your scripts.

{% include accordion_sample.html language="js" id="queue_js_1" title="Inquire" sample="queue_inq" %}

##<a name="Python"></a>Python
TODO!

##<a name="Java"></a>Java
TODO!

