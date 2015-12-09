{% highlight php %}
<?php
	/*
	 * Inquire all authority records for queuemanager PIGEON.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/authrec/inquire/PIGEON/?Options=Name%20All%20Matching&Options=Entity%20Explicit";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo "An HTTP error occurred while getting authority records: $err\n";
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo "An MQ error occurred while inquiring authority records.\n";
			echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
		}
		else {
			if ( isset($json->authrecs) && count($json->authrecs) > 0 ) {
				foreach($json->authrecs as $authrec)
				{
					echo $authrec->ProfileName->value;
					echo '(';
					echo $authrec->ObjectType->display;
					echo ')';
					echo ' - ';
					echo $authrec->EntityName->value;
					echo "\n";
					
					$authorizations = array();
					foreach($authrec->AuthorizationList->value as $authorization)
					{
						$authorizations[] = $authorization->display;
					}
					echo "   ";
					echo join(', ', $authorizations);
					echo "\n";
				}
			}
			else
			{
				echo "No authority records found\n";
			}
		}
	}
{% endhighlight %}
