{% highlight php %}
<?php

	/*
	 * Inquire all cluster information from queuemanager PIGEON.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/clusqmgr/inquire/PIGEON";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo "An HTTP error occurred while inquiring cluster queuemanagers: $err\n";
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo "An MQ error occurred while inquiring cluster queuemanagers.\n";
			echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
		}
		else {
			if ( isset($json->clusqmgrs) && count($json->clusqmgrs) > 0 ) {
				foreach($json->clusqmgrs as $clusqmgr)
				{
					echo $clusqmgr->ClusterName->value;
					echo " : ";
					echo $clusqmgr->QMgrName->value;
					echo "\n";
				}
			}
			else
			{
				echo "No cluster queuemanagers found\n";
			}
		}
	}
{% endhighlight %}
