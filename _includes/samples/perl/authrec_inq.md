{% highlight perl %}
#!/usr/bin/perl
use strict;
use warnings;
use feature qw(say);

use JSON;
use LWP::UserAgent;
use HTTP::Request::Common;

# This sample will show all authentication records and will list
# the profilename together with the entityname and the given authorizations.

my $qmgr = shift;
die("Please pass me the name of a queuemanager as argument") 
	unless defined($qmgr);

my $json = JSON->new;

my %input = ( 
	'Options' => [ 
		'Name All Matching', 
		'Entity Explicit'
	]
);
my $content = $json->encode(\%input);    

my $ua = LWP::UserAgent->new;
my $req = POST 'http://localhost:8081/api/authrec/inquire/' . $qmgr;
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
	foreach my $authrec(@{$mqweb->{authrecs}}) {
		my $output = $authrec->{ProfileName}->{value};
		$output .= '(' . $authrec->{ObjectType}->{display} . ')';
		$output .= ' - ' . $authrec->{EntityName}->{value};

		my @authorizations = ();
		foreach my $authority(@{$authrec->{AuthorizationList}->{value}}) {
			push(@authorizations, $authority->{display});
		}
		$output .= ' : ' . join(', ', @authorizations);

		say $output;
	}
}

{% endhighlight %}
