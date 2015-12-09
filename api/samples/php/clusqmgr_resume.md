{% highlight php %}
<?php

	/*
	 * Resume queuemanager PIGEON in cluster BIRDS.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = 'http://localhost:8081/api/clusqmgr/resume/PIGEON/BIRDS';

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo 'An HTTP error occurred while trying to resume: ', $err, PHP_EOL;
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo 'An MQ error occurred while trying to resume.', PHP_EOL;
			echo 'Reason Code: ', 
				$json->error->reason->code,
				' - ', 
				$json->error->reason->desc,
				PHP_EOL;
		}
		else {
			echo 'Queuemanager PIGEON resumed in clusterd BIRDS', PHP_EOL;
		}
	}
{% endhighlight %}
