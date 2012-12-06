#!c:/perl/bin/perl.exe
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
		$host = "192.168.$oct3.$oct4";
		my $t = threads->new( \&sokent_send, $host );
		push( @threads, $t );
		if ( ($i % 60) == 0 ) {
			foreach my $tr (@threads) {
				eval { $tr->join(); };
				if ($@) {
					;
					#print "\n" . $@ . "\n";
				}
			}
			undef @threads;			
			print STDERR $host;
		}
		$oct4++;
	}

	foreach my $tr (@threads) {
		eval { $tr->join(); };
			if ($@) {
				;
				#print "\n" . $@ . "\n";
			}
	}
	undef @threads;

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
		print $sock "magicnumber=B16B00B5&func=settime";
		print STDERR "ok";
		close($sock);
		$| = 1;
	}
}

