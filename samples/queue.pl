#!/usr/bin/perl
use strict;
use warnings;
use LWP::UserAgent;
use HTTP::Request::Common;

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument") unless defined($qmgr);

my $json = '{ "QName" : "T*" }';

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/queue/inquire/' . $qmgr;    
$req->header( 'Content-Type' => 'application/json' );
$req->content($json);

my $res = $ua->request($req);

print $res->content();
