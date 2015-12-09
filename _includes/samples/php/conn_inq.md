{% highlight php %}
<?php
	/*
	 * Inquire all connections from queuemanager PIGEON.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/conn/inquire/PIGEON/";
	
	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo "An HTTP error occurred while getting connection information: $err\n";
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo "An MQ error occurred while inquiring connections.\n";
			echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
		}
		else {
			if ( isset($json->connections) && count($json->connections) > 0 ) {
				foreach($json->connections as $connection)
				{
					echo $connection->ApplTag->value;
					echo ' (';
					echo $connection->ApplType->display;
					echo ')';
					echo "\n";
				}
			}
			else
			{
				echo "No connections found\n";
			}
		}
	}
{% endhighlight %}
