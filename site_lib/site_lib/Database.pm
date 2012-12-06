use strict;
use warnings;
use DBI;
use Data::Dumper;
use CGI::Carp;

use constant { INSERT_NEW_SENSOR =>
	  "INSERT INTO Sensors time,auditory,id_sensor,value value(?,?,?,?);" };

my $host   = "localhost";
my $port   = "3306";
my $user   = "root";
my $pass   = "o21092";
my $dbname = "gravicapa";
my $driver = "mysql";

my $dbh = undef;

sub get_connection {
	unless ($dbh) {
		my $str = "DBI:$driver:$dbname:$host:$port";
		$dbh = ( DBI->connect( $str, $user, $pass ) );
		unless ($dbh) {
			die "error connect to database";
		}
	}
	return $dbh;
}

sub database_disconnect() {
	unless ($dbh) {
		return undef;
	}
	unless ( $dbh->ping ) {
		return undef;
	}
	$dbh->{PrintError} = 1;
	$dbh->disconnect();
	return 1;
}

sub died {
	my ($msg) = @_;
	unless ($msg) {
		$msg = " ";
	}
	if ($dbh) {
		database_disconnect();
	}
	$dbh = undef;
	confess( "\n message :" . $msg . "\n" );
}

sub do_sql($@) {

   #$self->dbh - connection to DB
   #$sql_query - query for execute. It can path to file or sql text query
   #$refarray_bind_param - reference for array parameter for query in $sql_query
   #return - count change rows
   #

	my ( $query, @params ) = @_;
	my $tdbh = get_connection();
	unless ($tdbh) {
		died("error for connection database ");
	}
	unless ( $tdbh->ping ) {
		died("error for connection database ");
	}
	unless ($query) {
		died("query for null ");
	}
	my $rev = $tdbh->do( $query, {}, @params );
	unless ($rev) {
		died( "query error: \" $query \" \n " . $tdbh->errstr );
	}
	return $rev;
}

sub exec_sql_fetchrow_array($@) {

	# $_[0] - query
	# $_[1] - array parameter for query
	#return ref for array ref hash
	my ( $query, @params ) = @_;
	my $tdbh = get_connection();
	unless ($tdbh) {
		died("error for connection database ");
	}
	unless ($query) {
		died("query for null ");
	}
	my $sth = $tdbh->prepare($query);
	my $rev = $sth->execute(@params);
	unless ($rev) {
		died( "query error: \" $query \" \n " . $sth->errstr );
		$sth->finish();
	}
	if ( $rev == 0 ) {

		# if query SELECT return 0 rows, execute return 0E0, and
		# we do not go to the unless, but we'll go to the this if
		$sth->finish();
		return (undef);
	}
	my @response = $sth->fetchrow_array();
	$sth->finish();
	unless (@response) {
		@response = (undef);
	}
	return @response;
}

sub select_sql($@) {

	# $_[0] - query
	# $_[1] - reference for array parameter for query
	#return ref for array ref hash
	my ( $query, @params ) = @_;
	my $tdbh = get_connection();
	unless ($tdbh) {
		died("error for connection database ");
	}
	unless ($query) {
		died("query for null ");
	}
	my $sth = $tdbh->prepare($query);
	unless ($sth) {
		died("fail prepare query $query ");
	}
	my $rev = $sth->execute(@params);
	unless ($rev) {
		died( "query error: \" $query \" \n " . $sth->errstr );
		$sth->finish();
	}
	if ( $rev == 0 ) {

		# if query SELECT return 0 rows, execute return 0E0, and
		# we do not go to the unless, but we'll go to the this if
		$sth->finish();
		return 0;
	}
	my @response = ();
	while ( my $response_ref = $sth->fetchrow_hashref() ) {
		push( @response, $response_ref );
	}
	$sth->finish();
	return \@response;
}

sub exec_sql($@) {
	my $rev;
	unless ( $rev = select_sql(@_) ) {
		died("execute return 0 rows ");
	}
	return $rev;
}
1;

