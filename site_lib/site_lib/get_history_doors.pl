#!c:/perl/bin/perl.exe
use strict;
use CGI qw(:all escape);
use Database;

use constant { GET_VALUES_SENSORS =>
"SELECT * FROM Parametrs WHERE id_sensor=? ORDER BY time DESC limit 100;",
};

my $query_cgi = CGI::new();

print "Content-type: text/plain; charset=UTF8\n\n";

my $arr = select_sql( GET_VALUES_SENSORS, $query_cgi->param("id") );
if ($arr) {
	my @sensors = @{$arr};
	print "<table border='1px' style=\"width: 100%;\">";
	foreach my $sensor (@sensors) {

		my $val_sen = $sensor->{value};
		my $desc = $sensor->{description};
		if ( not defined $desc ) {
			$desc = "Описание отсутствует";
		}
		print "
			  <tr id="
		  . $sensor->{id_sensor}
		  . " class=\"sensorstable\" style=\"font-size: 15px;\"> 
				  <td style=\"display : none;\"> 
				  " . $sensor->{id_sensor} . "
				  </td>
				  <td>" . $desc . "</td>";
		if ( not defined $val_sen ) {
			print "<td >" . "ХЗ" . "</td>";
		}
		elsif ( $val_sen == 0 ) {
			print "<td style=\"background-color: #71c73e;\">"
			  . "Открыто" . "</td>";
		}
		elsif ( $val_sen == 1 ) {
			print "<td style=\"background-color: #FF1010;\">"
			  . "Закрыто" . "</td>";
		}
				
		print "<td>".$sensor->{'time'}."</td>";
		print "</td></tr>";
	}

	print "</table>";
}

