#!/bin/perl
#use Socket;
use IO::Socket;
use threads;

print "Content-type: text/plain; charset=UTF8\n\n";


my $oct3     = 0;
my $oct4     = 1;
my $host     = "";
my $runtread = 0;
my @threads;
for ( my $j = 0 ; $j < 255 ; $j++ ) {
	for ( my $i = 0 ; $i < 255 ; $i++ ) {
		$host = "127.0.5.$oct4";
		my $t = threads->new( \&sokent_send, $host );

		push( @threads, $t );
		if ( ($i % 100) == 0 ) {
			foreach my $tr (@threads) {
				eval { $tr->join(); };
				if ($@) {
					;
					#print "\n" . $@ . "\n";
				}
			}
			undef @threads;
		}

		$oct4++;
	}

	#print STDERR $host;
	$oct4 = 0;
	$oct3++;
}

exit 0;

sub sokent_send {
	my $host_th = @_[0];
	my $sock    = new IO::Socket::INET(
		PeerAddr => $host_th,
		PeerPort => 12345,
		Proto    => 'tcp',
	);
	if ($sock) {
		print $sock "GET /cgi/printenv.pl? HTTP/1.1 

			Host: serv2 
			\n\n";
		close($sock);
		$| = 1;
	}
}

