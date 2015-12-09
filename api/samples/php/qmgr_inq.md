{% highlight php %}
<?php
	/*
	 * This sample will show the description of queuemanager PIGEON.
	 * MQWeb runs on localhost and is listening on port 8081. 
	 */
	$url = "http://localhost:8081/api/qmgr/inquire/PIGEON";

	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	if ( ($response = curl_exec($curl)) === false )	{
		$err = curl_error($curl);
		echo 'An HTTP error occurred while inquiring queuemanager status: '
			. $err
			. PHP_EOL;
	}
	else {
		$json = json_decode($response);
		if ( isset($json->error) ) {
			echo 'An MQ error occurred while inquiring queuemanager status.'
				. PHP_EOL;
			echo 'Reason Code: '
				. $json->error->reason->code
				. ' - '
				. $json->error->reason->desc
				. PHP_EOL;
		}
		else {
			if ( isset($json->qmgr) ) {
				echo $json->qmgr->QMgrName->value
					. ' : '
					. $json->qmgr->QMgrDesc->value
					. PHP_EOL;
			}
			else {
				echo 'No qmgr found' . PHP_EOL;
			}
		}
	}
{% endhighlight %}
