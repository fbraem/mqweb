<?php
	/*
	 * This sample will show all SYSTEM queues from queuemanager PIGEON and
	 * prints the current queue depth if this property exists for the queue.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/queue/inquire/PIGEON/SYSTEM*";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo "An HTTP error occurred while getting queue information: $err\n";
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo "An MQ error occurred while inquiring queues.\n";
			echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
		}
		else {
			if ( isset($json->data) && count($json->data) > 0 ) {
				foreach($json->data as $queue)
				{
					echo $queue->QName->value;
					if ( isset($queue->CurrentQDepth) ) {
						echo ': ' . $queue->CurrentQDepth->value;
					}
					echo "\n";
				}
			}
			else
			{
				echo "No queues found\n";
			}
		}
	}
