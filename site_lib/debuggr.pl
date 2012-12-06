#!c:/perl/bin/perl.exe

use strict;

use JSON;
use Data::Dumper;


print "Content-type: text/plain; charset=UTF8\n\n";

my $json_text = '{
	"data":[
	[1354616367000, 0], [1354616372000, 1], [1354616377000, 7], [1354616381000, 8],
        [1354616870000, 13], [1354616875000, 23], [1354617842000, 32], [1354617846000, 33]
]
}
';

#my @decoded_json = decode_json($json_text);

#print Dumper(@decoded_json);

#my $json = encode_json \@decoded_json;

print $json_text;
