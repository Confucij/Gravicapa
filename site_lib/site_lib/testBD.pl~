use strict;
use warnings;
use DBI;
use Data::Dumper;

my $host="localhost";
my $port="3306";
my $user="root";
my $pass="o21092";
my $dbname="gravicapa";
my $driver="mysql";

my $str="DBI:$driver:$dbname:$host:$port";

my $dbh= ( DBI->connect($str,$user,$pass) );
            unless($dbh){
                print "error connect to database";
            }
my $query="select * from Sensors;";			
my $sth = $dbh->prepare($query);
            my $rev=$sth->execute();
			my @response = $sth->fetchrow_array();
            print Dumper(@response);
