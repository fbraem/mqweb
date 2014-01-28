<?php

// Don't forget to install Guzzle before running this sample!
// All info: http://docs.guzzlephp.org/

require_once 'vendor/autoload.php';
use Guzzle\Http\Client;

// Create a client and provide a base URL
$client = new Client('http://localhost:8081');
$data = $client->get('queue/inquire/PIGEON')->send()->json();
print_r($data);

