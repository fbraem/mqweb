#!/usr/bin/perl
use strict;
use warnings;
use LWP::UserAgent;
use HTTP::Request::Common;


my $json = '{ "QName" : "T*" }';

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/queue/inquire/EAGLE';    
$req->header( 'Content-Type' => 'application/json' );
$req->content($json);

my $res = $ua->request($req);

print $res->content();