{% highlight php %}
<?php
	/*
	 * Inquire all SYSTEM authentication information objects from queuemanager 
	 * PIGEON. MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/authinfo/inquire/PIGEON/SYSTEM*";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo "An HTTP error occurred while inquiring authentication information: $err\n";
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo "An MQ error occurred while inquiring authentication information objects.\n";
			echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
		}
		else {
			if ( isset($json->authinfos) && count($json->authinfos) > 0 ) {
				foreach($json->authinfos as $authinfo)
				{
					echo $authinfo->AuthInfoName->value;
					echo "\n";
				}
			}
			else
			{
				echo "No authentication information objects found\n";
			}
		}
	}
{% endhighlight %}
