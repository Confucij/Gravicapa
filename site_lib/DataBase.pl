
use strict;
use warnings;
use DBI;
use Data::Dumper;

    my $dbh;

        sub test(){
	my $str="DBI:mysql:gravicapa:localhost:3306";
        
	$dbh =  DBI->connect($str,"root","o21092");
            
			my $sth = $dbh->prepare("Select * from Parametrs");
            my $rev=$sth->execute();
			my @response = $sth->fetchrow_array();
            print Dumper(@response);
			$sth->finish();
            			
			}


       
        
test();		
