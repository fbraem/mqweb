{% highlight php %}
<?php
	/*
	 * Inquire status of all listeners from queuemanager 
	 * PIGEON. MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/lsstatus/inquire/PIGEON";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	$response = curl_exec($curl);
	$data = json_decode($response, true);		
	print_r($data);
{% endhighlight %}
