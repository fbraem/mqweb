#!/usr/bin/perl
use strict;
use warnings;
use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument") unless defined($qmgr);

my $json = JSON->new;

my %input = ( 
  'ChannelName' => '*',
  'IntegerFilterCommand' => {
  	'Parameter' => 'ChannelType',
  	'Operator' => 'EQ',
  	'FilterValue' => 'Server-connection'
  }
);

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/chstatus/inquire/' . $qmgr;    
$req->header( 'Content-Type' => 'application/json' );
$req->content($json->encode(\%input));

my $res = $ua->request($req);

print $json->pretty->encode($json->decode($res->content()));
