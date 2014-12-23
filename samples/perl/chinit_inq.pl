#!/usr/bin/perl
use strict;
use warnings;
use feature qw(say);

use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;

# Get channel initiation information

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument")
	unless defined($qmgr);

my $json = JSON->new;

my %input = ();
my $content = $json->encode(\%input);

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/chinit/inquire/' . $qmgr;
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
	foreach my $response(@{$mqweb->{response}}) {
		if ( exists($response->{TransportType})
			   && $response->{TransportType}->{display} eq 'TCP' ) {
			my $ip = '';
			if ( exists ($response->{IPAddress}) ) {
				$ip = $response->{IPAddress}->{value};
			}	else {
				$ip = '?';
			}
			$ip .= ':';
			if ( exists ($response->{Port}) ) {
				$ip .= $response->{Port}->{value};
			}	else {
				$ip .= '?';
			}

			say $response->{TransportType}->{display}
				, ' - '
				, $ip
				, ' - '
				, $response->{Status}->{display};
		}
	}
}
