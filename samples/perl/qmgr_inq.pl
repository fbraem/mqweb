#!/usr/bin/perl
use strict;
use warnings;
use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;
use feature qw(say);

# This sample will show the description of the queuemanager

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument") 
	unless defined($qmgr);

my $json = JSON->new;

my %input = ( 
	'QMgrAttrs' => [
		'QMgrDesc'
	],
);
my $content = $json->encode(\%input);    

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/qmgr/inquire/' . $qmgr;
$req->header(
	'Content-Type' => 'application/json',
	'Content-length' => length($content)
);
$req->content($content);

my $res = $ua->request($req);
die $res->status_line unless $res->is_success;

my $mqweb = $json->decode($res->content());
if ( exists($mqweb->{error}) ) {
	say 'An MQ error occurred while inquiring queuemanager.';
	say	'Reason Code: ',
		$mqweb->{error}->{reason}->{code},
		' - ',
		$mqweb->{error}->{reason}->{desc};
}
else {
	say $mqweb->{data}->[0]->{QMgrName}->{value},
		' : ', 
		$mqweb->{data}->[0]->{QMgrDesc}->{value};
}

