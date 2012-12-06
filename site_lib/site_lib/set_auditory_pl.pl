#!c:/perl/bin/perl.exe

use strict;
use CGI qw(:all escape);
use Database;
use Data::Dumper;
use constant {
	SET_AUDITORY => "UPDATE Sensors SET auditory=?  WHERE id_sensor=?;"
};


my $query_cgi=CGI::new();
 
 do_sql(SET_AUDITORY,$query_cgi->param("auditory"),$query_cgi->param("id"));


print "Content-type: text/plain; charset=UTF8\n\n";
print "0";




