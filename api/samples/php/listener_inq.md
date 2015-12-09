{% highlight php %}
<?php
	/*
	 * Inquire all SYSTEM listeners with transporttype TCP from queuemanager 
	 * PIGEON. MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/listener/inquire/PIGEON/SYSTEM*/TCP";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);


	$response = curl_exec($curl);
	$data = json_decode($response, true);		
	print_r($data);
{% endhighlight %}
