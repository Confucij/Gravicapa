use strict;
use CGI qw(:all escape);
use Database;
use CGI::Cookie;
use Data::Dumper;
use Digest::SHA1 qw(sha1);
use constant {
	CHECK_USER => "SELECT count(*) FROM Users WHERE login=? AND pass=?" };


sub authorization {
	my ( $login, $pass ) = @_;

	#sha1($pass);
	my ($val_sen) = exec_sql_fetchrow_array( CHECK_USER, $login, $pass );
	if ( not defined $val_sen ) { $val_sen = 0; }
	return $val_sen;
}

sub sentcook {
	my $c = new CGI::Cookie(
		-name  => "value",
		-value => { "2" => sha1( $_[1] ), "1" => $_[0] }
	);

	print header( -cookie => $c );
}

sub killcook {
	my $c = new CGI::Cookie(
		-name  => "value",
		-value => { "2" => sha1( $_[1] ), "1" => $_[0] }
		-expires => -1,
	);
	print header( -cookie => $c );
}

sub redirect {
	my $url = $_[0];
	print "Location: $url\n\n";
}

1;
