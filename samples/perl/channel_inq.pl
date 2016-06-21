#!/usr/bin/perl
use strict;
use warnings;
use feature qw(say);

use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;

# Get all channels and list them with the channeltype and connectionname (if
# available

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument") 
	unless defined($qmgr);

my $json = JSON->new;

my %input = ( 
	'ChannelName' => '*',
	'ChannelAttrs' => [ 
		'ConnectioName'
		# No need to specify ChannelName and ChannelType, they are always
		# returned.
	]
);
my $content = $json->encode(\%input);    

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/channel/inquire/' . $qmgr;    
$req->header(
	'Content-Type' => 'application/json',
	'Content-length' => length($content)
);
$req->content($content);

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
	foreach my $channel(@{$mqweb->{data}}) {
		my $output = $channel->{ChannelName}->{value};
		$output .= '(' . $channel->{ChannelType}->{display} . ')';
		if ( exists($channel->{ConnectionName}) ) {
			my $connectionName = $channel->{ConnectionName}->{value};
			if ( length($connectionName) > 0 ) {
				$output .= ' : ' . $connectionName;
			}
			else {
				$output .= ' : - ';
			}
		}
		say $output;
	}
}

