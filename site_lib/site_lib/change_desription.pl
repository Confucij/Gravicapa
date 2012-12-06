#!c:/perl/bin/perl.exe

use strict;
use CGI qw(:all escape);
use Database;
use Data::Dumper;
use constant {
	GET_DESRIPTION => "SELECT description,auditory,value  FROM Sensors WHERE id_sensor=?",
	SET_DESRIPTION	 => "UPDATE Sensors SET description=?,auditory=?,value=?  WHERE id_sensor=?",
	 };

my $query_cgi = CGI::new();

print "Content-type: text/plain; charset=UTF8\n\n";

my $query_cgi = CGI::new();


do_sql(SET_DESRIPTION,$query_cgi->param("desription"),$query_cgi->param("auditory"),$query_cgi->param("type"),$query_cgi->param("id"));

my ($desripition,$auditory,$value) =
  exec_sql_fetchrow_array( GET_DESRIPTION, $query_cgi->param("id") );

if ( defined $desripition ) {
	print $desripition."&1";
}else{
	print "Описание отсутствует&1";
}
print $auditory."&2";
print $value."&3";
