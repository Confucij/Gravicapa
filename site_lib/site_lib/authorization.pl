#!c:/perl/bin/perl.exe

use strict;
use CGI qw(:all escape);
use Authorization;

my $query=CGI::new();

if (authorization($query->param("login"),$query->param("pass"))){
	sentcook($query->param("login"),$query->param("pass"));
	print "ok";
}else{
	print "Content-type: text/plain; charset=UTF8\n\n";
	print "fail";
	print $query->param("pass");
}

1;
