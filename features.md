---
layout: default
title: MQWeb &bull; Features
features: true
showSidebar: true
---
Features
========

MQWeb can be used in two ways:

Built-in HTML
-------------

MQWeb has its own HTML pages which can be viewed by surfing to the web controller.
This controller can be reached by using `web` as first part of the URL:

    http://<mqwebhost:port>/web/qmgr/PIGEON

The built-in HTML pages use [AngularJS](http://angularjs.org) to call the API's
of MQWeb and to transform the JSON objects into HTML.

> **Note:** this will change for version 0.1.0 and above. The built-in HTML pages
> are removed from the MQWeb repository and the web application will be rewritten
> in the [MQWeb App](https://github.com/fbraem/mqwebapp) repository.

API
---

The web [API]({{ site.baseurl }}/api/index.html) of MQWeb sends a PCF command 
to WebSphere MQ and returns the answer as JSON objects. The 
[API]({{ site.baseurl }}/api/index.html) can be reached by using `api` as first 
part of the URL:

	http://<mqwebhost:port>/api/queue/inquire/PIGEON

The API's can be called from any language that supports HTTP clients:

{% highlight php %}
<?php
   $url = "http://localhost:8081/api/queue/inquire/PIGEON";
   $curl = curl_init();
   curl_setopt($curl, CURLOPT_URL, $url);
   curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
   $response = curl_exec($curl);
   $data = json_decode($response, true);
{% endhighlight %}

or in Python

{% highlight python %}
  conn = httplib.HTTPConnection('localhost', 8081)
  conn.request('GET', '/api/queue/inquire/PIGEON')
  res = conn.getresponse()
  data = json.loads(res.read())
{% endhighlight %}

Look at [API overview](/api/index.html) to see which api's are available.

