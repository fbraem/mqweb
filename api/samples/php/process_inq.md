{% highlight php %}
<?php
	/*
	 * This sample will show all SYSTEM processes.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/process/inquire/PIGEON/SYSTEM*";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo "An HTTP error occurred while getting process information: $err\n";
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo "An MQ error occurred while inquiring processes.\n";
			echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
		}
		else {
			if ( isset($json->processes) && count($json->processes) > 0 ) {
				foreach($json->processes as $process)
				{
					echo $process->ProcessName->value;
					echo "\n";
				}
			}
			else
			{
				echo "No processes found\n";
			}
		}
	}
{% endhighlight %}
