{% highlight perl %}
#!/usr/bin/perl
use strict;
use warnings;
use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;

# This sample will show the status of all listeners

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument") 
	unless defined($qmgr);

my $json = JSON->new;

# There's no need to set ListenerName to * because this is the default,
# but we do it here to show how to build the json content.
my %input = ( 
	'ListenerName' => '*',
);
my $content = $json->encode(\%input);    

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/lsstatus/inquire/' . $qmgr;
$req->header(
	'Content-Type' => 'application/json',
	'Content-length' => length($content)
);
$req->content($content);

my $res = $ua->request($req);
die $res->status_line unless $res->is_success;

my $mqweb = $json->decode($res->content());
if ( exists($mqweb->{error}) ) {
	say 'An MQ error occurred while inquiring listener status.';
	say 'Reason Code: '
		, $mqweb->{error}->{reason}->{code}
		, ' - '
		, $mqweb->{error}->{reason}->{desc};
}}
else {
	foreach my $status(@{$mqweb->{statuses}}) {
		print $status->{ListenerName}->{value};
		print ' : ', $status->{Status}->{display}; 
		print "\n";
	}
}

{% endhighlight %}
