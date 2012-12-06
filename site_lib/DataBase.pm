
package DataBase;
{

use strict;
use warnings;
use DBI;
use Data::Dumper;

    my $dbh;

        sub test(){
 my $str="DBI:mysql:gravicapa:localhost:3306";
        
	$dbh =  DBI->connect($str,"root","o21092");
            
			my $sth = $dbh->prepare($query);
            my $rev=$sth->execute(@params);
			my @response = $sth->fetchrow_array();
            print Dumper(@response);
			$sth->finish();
            			
			}


       
        
		

}
1;
