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

my $json = JSON->new;

my %input = ( 
  'ClusterQMgrName' => '*',
  'ClusterName' => '*'
);
my $content = $json->encode(\%input);    

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/clusqmgr/inquire/' . $qmgr;    
$req->header(
	'Content-Type' => 'application/json',
	'Content-length' => length($content)
);
$req->content($json->encode(\%input));

my $res = $ua->request($req);

my $mqweb = $json->decode($res->content());
if ( exists($mqweb->{error}) ) {
	say 'An MQ error occurred while inquiring queues.';
	say 'Reason Code: '
		, $mqweb->{error}->{reason}->{code}
		, ' - '
		, $mqweb->{error}->{reason}->{desc};
}
else {
	my %clusters;
	foreach my $clusqmgr(@{$mqweb->{clusqmgrs}}) {
		my $clusterName = $clusqmgr->{ClusterName}->{value};
		push(@{$clusters{$clusterName}}, $clusqmgr->{QMgrName}->{value});
	}

	foreach my $cluster(keys(%clusters))
	{
	  say $cluster;
	  say '-' x length($cluster);
	  foreach my $qmgr(@{$clusters{$cluster}})
	  {
	    say '  ', $qmgr;
	  }
	}
}

