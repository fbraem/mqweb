<?php
	/*
	 * This sample will show all SYSTEM namelists from queuemanager PIGEON with
	 * their description.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/nl/inquire/PIGEON/SYSTEM*";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo "An HTTP error occurred while getting namelist information: $err\n";
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo "An MQ error occurred while inquiring namelist.\n";
			echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
		}
		else {
			if ( isset($json->data) && count($json->data) > 0 ) {
				foreach($json->data as $namelist)
				{
					echo $namelist->NamelistName->value;
					echo ': ';
					echo $namelist->NamelistDesc->value;
					echo "\n";
				}
			}
			else
			{
				echo "No namelists found\n";
			}
		}
	}
