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
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-1">Inquire</a>
    <div id="accordion-php-1" class="accordion-section-content">
      {% include_relative php/authinfo_inq.html %}
    </div>
  </div>
</div>

###Authority Records
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-2">Inquire</a>
    <div id="accordion-php-2" class="accordion-section-content">
      {% include_relative php/authrec_inq.html %}
    </div>
  </div>
</div>

###Channels
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-3">Inquire</a>
    <div id="accordion-php-3" class="accordion-section-content">
      {% include_relative php/channel_inq.html %}
    </div>
  </div>
</div>

###Channel Statuses
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-4">Inquire</a>
    <div id="accordion-php-4" class="accordion-section-content">
      {% include_relative php/chstatus_inq.html %}
    </div>
  </div>
</div>

###Listeners
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-5">Inquire</a>
    <div id="accordion-php-5" class="accordion-section-content">
      {% include_relative php/listener_inq.html %}
    </div>
  </div>
</div>

###Listener Statuses
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-6">Inquire</a>
    <div id="accordion-php-6" class="accordion-section-content">
      {% include_relative php/lsstatus_inq.html %}
    </div>
  </div>
</div>

###Namelists
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-7">Inquire</a>
    <div id="accordion-php-7" class="accordion-section-content">
      {% include_relative php/namelist_inq.html %}
    </div>
  </div>
</div>

###Processes
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-8">Inquire</a>
    <div id="accordion-php-8" class="accordion-section-content">
      {% include_relative php/process_inq.html %}
    </div>
  </div>
</div>

###QueueManager
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-9">Inquire</a>
    <div id="accordion-php-9" class="accordion-section-content">
      {% include_relative php/qmgr_inq.html %}
    </div>
  </div>
</div>

###QueueManager Status
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-10">Inquire</a>
    <div id="accordion-php-10" class="accordion-section-content">
      {% include_relative php/qmstatus_inq.html %}
    </div>
  </div>
</div>

###Queues
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-11">Inquire</a>
    <div id="accordion-php-11" class="accordion-section-content">
      {% include_relative php/queue_inq.html %}
    </div>
  </div>
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-12">Inquire (with Guzzle)</a>
    <div id="accordion-php-12" class="accordion-section-content">
      {% include_relative php/guzzle.html %}
    </div>
  </div>
</div>

###Services
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-php-13">Inquire</a>
    <div id="accordion-php-13" class="accordion-section-content">
      {% include_relative php/service_inq.html %}
    </div>
  </div>
</div>

<a name="Perl"></a>Perl
----
With [Perl](https://www.perl.org/) you can use
[LWP](http://search.cpan.org/dist/libwww-perl/lib/LWP.pm) together with
[HTTP::Request::Common](http://search.cpan.org/~gaas/HTTP-Message-6.06/lib/HTTP/Request/Common.pm) 
and [JSON](http://search.cpan.org/~makamaka/JSON-2.90/lib/JSON.pm)
or any other [http client](https://www.google.be/#q=perl+http+client) implementation.

###Authentication Information
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-1">Inquire</a>
    <div id="accordion-perl-1" class="accordion-section-content">
      {% include_relative perl/authinfo_inq.html %}
    </div>
  </div>
</div>

###Authority Records
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-2">Inquire</a>
    <div id="accordion-perl-2" class="accordion-section-content">
      {% include_relative perl/authrec_inq.html %}
    </div>
  </div>
</div>

###Channels
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-3">Inquire</a>
    <div id="accordion-perl-3" class="accordion-section-content">
      {% include_relative perl/channel_inq.html %}
    </div>
  </div>
</div>

###Channel Statuses
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-4">Inquire</a>
    <div id="accordion-perl-4" class="accordion-section-content">
      {% include_relative perl/chstatus_inq.html %}
    </div>
  </div>
</div>

###Listeners
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-5">Inquire</a>
    <div id="accordion-perl-5" class="accordion-section-content">
      {% include_relative perl/listener_inq.html %}
    </div>
  </div>
</div>

###Listener Statuses
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-6">Inquire</a>
    <div id="accordion-perl-6" class="accordion-section-content">
      {% include_relative perl/lsstatus_inq.html %}
    </div>
  </div>
</div>

###Namelists
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-7">Inquire</a>
    <div id="accordion-perl-7" class="accordion-section-content">
      {% include_relative perl/namelist_inq.html %}
    </div>
  </div>
</div>

###Processes
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-8">Inquire</a>
    <div id="accordion-perl-8" class="accordion-section-content">
      {% include_relative perl/process_inq.html %}
    </div>
  </div>
</div>

###QueueManager
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-9">Inquire</a>
    <div id="accordion-perl-9" class="accordion-section-content">
      {% include_relative perl/qmgr_inq.html %}
    </div>
  </div>
</div>

###QueueManager Status
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-10">Inquire</a>
    <div id="accordion-perl-10" class="accordion-section-content">
      {% include_relative perl/qmstatus_inq.html %}
    </div>
  </div>
</div>

###Queues
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-11">Inquire</a>
    <div id="accordion-perl-11" class="accordion-section-content">
      {% include_relative perl/queue_inq.html %}
    </div>
  </div>
</div>

###Services
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-perl-12">Inquire</a>
    <div id="accordion-perl-12" class="accordion-section-content">
      {% include_relative perl/service_inq.html %}
    </div>
  </div>
</div>

<a name="Ruby"></a>Ruby
----
The Ruby Standard Library contains [Net/HTTP](http://ruby-doc.org/stdlib/libdoc/net/http/rdoc/)
that implements an HTTP client API for Ruby. Or you can use one of the
[HTTP clients](https://www.ruby-toolbox.com/categories/http_clients)
 from the [Ruby Toolbox](https://www.ruby-toolbox.com).

###Queues
<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-ruby-1">Inquire</a>
    <div id="accordion-ruby-1" class="accordion-section-content">
      {% include_relative ruby/queue_inq.html %}
    </div>
  </div>
</div>

<a name="JavaScript"></a>JavaScript
----------
JavaScript can be used on the client side and on server side. In a browser
you can use [AJAJ](http://en.wikipedia.org/wiki/AJAJ) to call the MQWeb API's, 
on the server you can use [nodejs](http://nodejs.org/) to run your scripts.

<div class="accordion">
  <div class="accordion-section">
    <a class="accordion-section-title" href="#accordion-js-1">Inquire</a>
    <div id="accordion-js-1" class="accordion-section-content">
      {% include_relative js/queue_inq.html %}
    </div>
  </div>
</div>


<a name="Python"></a>Python
------

<a name="Java"></a>Java
----


