#!/usr/bin/perl
use strict;
use warnings;
use feature qw(say);

use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;

# Get the channelstatus for all cluster sender channels

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument")
	unless defined($qmgr);

my $json = JSON->new;

my %input = ( 
  'ChannelName' => '*',
  'IntegerFilterCommand' => {
  	'Parameter' => 'ChannelType',
  	'Operator' => 'EQ',
  	'FilterValue' => 'Cluster-sender'
  }
);
my $content = $json->encode(\%input);    

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/chstatus/inquire/' . $qmgr;    
$req->header(
	'Content-Type' => 'application/json',
	'Content-length' => length($content)
);
$req->content($json->encode(\%input));

my $res = $ua->request($req);
die $res->status_line unless $res->is_success;

my $mqweb = $json->decode($res->content());
if ( exists($mqweb->{error}) ) {
	say 'An MQ error occurred while inquiring queues.';
	say 'Reason Code: '
		, $mqweb->{error}->{reason}->{code}
		, ' - '
		, $mqweb->{error}->{reason}->{desc};
}
else {
	foreach my $status(@{$mqweb->{statuses}}) {
		say $status->{ChannelName}->{value}
			, ' : '
			, $status->{ChannelStatus}->{display}
	}
}

