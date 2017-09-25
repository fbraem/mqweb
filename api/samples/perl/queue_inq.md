{% highlight perl %}
#!/usr/bin/perl
use strict;
use warnings;
use feature qw(say);

use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;

# This sample will show all SYSTEM queues from the given queuemanager and
# prints the current queue depth if this property exists for the queue.

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument") 
	unless defined($qmgr);

my $json = JSON->new;

my %input = ( 
	'QName' => 'SYSTEM*',
	'QAttrs' => [
		'CurrentQDepth'
		# No need to add QName, it is always returned
	]
);
my $content = $json->encode(\%input);    

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/queue/inquire/' . $qmgr;
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
	foreach my $queue(@{$mqweb->{data}}) {
		my $output = $queue->{QName}->{value};
		$output .= ' : ' . $queue->{CurrentQDepth}->{value} 
			if ( exists($queue->{CurrentQDepth}) );
		say $output;
	}
}
{% endhighlight %}
