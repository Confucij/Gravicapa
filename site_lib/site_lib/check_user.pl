#!c:/perl/bin/perl.exe

use strict;
use CGI qw(:all escape);
use Authorization;

my $query=CGI::new();


	killcook($query->param("login"),$query->param("pass"));


1;
