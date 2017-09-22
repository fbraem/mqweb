{% highlight php %}
<?php

	/*
	 * Inquire all client channels from queuemanager PIGEON.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/channel/inquire/PIGEON/*/Server-connection";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl) ) === false ) {
		$err = curl_error($curl);
		echo "An HTTP error occurred while inquiring channel information: $err\n";
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo "An MQ error occurred while inquiring channels.\n";
			echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
		}
		else {
			if ( isset($json->data) && count($json->data) > 0 ) {
				foreach($json->data as $channel)
				{
					echo $channel->ChannelName->value;
					echo "\n";
				}
			}
			else
			{
				echo "No client channels found\n";
			}
		}
	}
{% endhighlight %}
