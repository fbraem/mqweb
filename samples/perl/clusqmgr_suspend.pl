#!/usr/bin/perl
use strict;
use warnings;
use feature qw(say);

use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;

# List all known clusters with their queuemanagers

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument") 
	unless defined($qmgr);

my $cluster = shift;
die("Please pass me the name of a cluster as second argument")
	unless defined($cluster);

my $json = JSON->new;

my %input = ( 
  'ClusterName' => $cluster
);
my $content = $json->encode(\%input);    

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/clusqmgr/suspend/' . $qmgr;    
$req->header(
	'Content-Type' => 'application/json',
	'Content-length' => length($content)
);
$req->content($content);

my $res = $ua->request($req);

my $mqweb = $json->decode($res->content());
use Data::Dumper;
print Dumper($mqweb);
if ( exists($mqweb->{error}) ) {
	say 'An MQ error occurred while inquiring queues.';
	say 'Reason Code: '
		, $mqweb->{error}->{reason}->{code}
		, ' - '
		, $mqweb->{error}->{reason}->{desc};
}
else {
	say $qmgr, " is suspended in cluster ", $cluster;
}

