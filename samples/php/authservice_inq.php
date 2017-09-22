<?php

/*
 * Inquire all authority service information for a queuemanager.
 * MQWeb runs on localhost and is listening on port 8081. 
 */
if ( count($argv) < 2 ) {
	echo 'Please pass a queuemanager name as argument', PHP_EOL;
	exit;
}
$qmgr = $argv[1];

$url = 'http://localhost:8081/api/authservice/inquire/' . $qmgr;

$curl = curl_init();
curl_setopt($curl, CURLOPT_URL, $url);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

if ( ($response = curl_exec($curl)) === false )	{
	$err = curl_error($curl);
	echo 'An HTTP error occurred while getting authority service information from '
		, $qmgr
		, ': '
		, $err
		, PHP_EOL;
} else {
	$json = json_decode($response);
	// When there is an error object returned, something went wrong with
	// the WebSphere MQ command.
	if ( isset($json->error) ) {
		echo 'An MQ error occurred while inquiring  status from '
			, $qmgr
			, PHP_EOL;
		echo 'Reason Code: '
			,	$json->error->reason->code
			, " - "
			, $json->error->reason->desc
			, PHP_EOL;
	}
	else {
		if ( isset($json->data) && count($json->data) > 0 ) {
			foreach($json->data as $authservice)
			{
				foreach($authservice->ServiceComponent->value as $serviceComponent) {
					echo $serviceComponent, PHP_EOL;
				}
			}
		}
		else
		{
			echo "No authority services found\n";
		}
	}
}
