<?php
/*
 * This sample will show all topics from a queuemanager.
 * MQWeb runs on localhost and is listening on port 8081. 
 */
if ( count($argv) < 2 ) {
	echo 'Please pass a queuemanager name as argument', PHP_EOL;
	exit;
}

$qmgr = $argv[1];

$url = 'http://localhost:8081/api/topic/inquire/' . $qmgr . '/*';

$curl = curl_init();
curl_setopt($curl, CURLOPT_URL, $url);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

if ( ($response = curl_exec($curl)) === false )	{
	$err = curl_error($curl);
	echo 'An HTTP error occurred while getting topic information from '
		, $qmgr
		, ': '
		, $err
		, PHP_EOL;
}
else {
	$json = json_decode($response);
	
	// When there is an error object returned, something went wrong with
	// the WebSphere MQ command.
	if ( isset($json->error) ) {
		echo 'An MQ error occurred while inquiring topics from '
			, $qmgr
			, PHP_EOL;
		echo 'Reason Code: '
			,	$json->error->reason->code
			, " - "
			, $json->error->reason->desc
			, PHP_EOL;
	}
	else {
		echo 'Topics on ', $qmgr, PHP_EOL;
		echo '----------', str_repeat('-', strlen($qmgr)), PHP_EOL;
		if ( isset($json->topics) && count($json->topics) > 0 ) {
			foreach($json->topics as $topic)
			{
				echo $topic->TopicName->value, PHP_EOL;
			}
		}
		else
		{
			echo 'No topics found', PHP_EOL;
		}
	}
}
