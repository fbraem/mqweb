<?php

	/*
	 * Inquire all server connection channel statuses for queuemanager PIGEON.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/chstatus/inquire/PIGEON?ChannelType=Server-connection";
	
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
			if ( isset($json->statuses) && count($json->statuses) > 0 ) {
				foreach($json->statuses as $status)
				{
					echo $status->ChannelName->value;
					echo " : ";
					echo $status->ChannelStatus->display;
					echo "\n";
				}
			}
			else
			{
				echo "No statuses found\n";
			}
		}
	}
