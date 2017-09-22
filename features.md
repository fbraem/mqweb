---
layout: default
title: MQWeb &bull; Features
features: true
showSidebar: true
---
Features
========

MQWeb can be used in two ways:

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

MQWeb App
---------

MQWeb App is a sample web application which has its own
[Github repository](https://github.com/fbraem/mqwebapp).

MQWeb API's can be integrated in your website using any server side language
that supports HTTP requests, but with the MQWeb app, you can use any client side
solution like [AngularJS](https://angularjs.org/),
[vue.js](http://www.vuejs.org), ...
