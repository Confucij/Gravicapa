#!c:/perl/bin/perl.exe
use strict;
use CGI qw(:all escape);
use Database;
use CGI::Cookie;
use Data::Dumper;
use Digest::SHA1 qw(sha1);
use constant { GET_CURRENT_USER => "SELECT pass FROM Users WHERE login=? " };

my $query   = new CGI;
my %cookies = fetch CGI::Cookie;
my $cook    = $cookies{"value"};
print "Content-type: text/plain; charset=UTF8\n\n";
if ($cook) {
	my $hashref = $cook->{value};
	my @hash    = @{$hashref};
	my ($pass) = exec_sql_fetchrow_array( GET_CURRENT_USER, $hash[1] );
	if ( sha1($pass) eq $hash[3] ) {
		print $hash[1];
	}
	else {
		print "0";
	}
}
else {
	print "0";
}
