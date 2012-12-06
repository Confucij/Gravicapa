#!c:/perl/bin/perl.exe

use strict;
use CGI qw(:all escape);
use Controler;
use Data::Dumper;


my $query_cgi=CGI::new();
 
my $func=$query_cgi->param("func");

print "Content-type: text/plain; charset=UTF8\n\n";

if($func=='reg'){
	
}elsif($func=='setdata'){
	
}elsif($func=='gettime'){
	
}