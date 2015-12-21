<?php
        /*
         * Start a channel on a queuemanager
         */

        if ( count($argv) < 3 ) {
                echo 'Please pass a queuemanager name and channelname as argument', PHP_EOL;
                exit;
        }
        $qmgr = $argv[1];
        $channel = $argv[2];

        $url = "http://localhost:8081/api/channel/start/$qmgr/$channel";

        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, $url);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

        if ( ($response = curl_exec($curl) ) === false ) {
                $err = curl_error($curl);
                echo "An HTTP error occurred while starting channel $channel: $err\n";
        }
        else {
                $json = json_decode($response);
                if ( isset($json->error) ) {
                        echo "An MQ error occurred while starting channel $channel.\n";
                        echo "Reason Code: {$json->error->reason->code} - {$json->error->reason->desc}\n";
                }
                else {
                        echo "Channel $channel started.\n";
                }
        }
