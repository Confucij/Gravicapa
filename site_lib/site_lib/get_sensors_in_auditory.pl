#!c:/perl/bin/perl.exe

use strict;
use CGI qw(:all escape);
use Database;
use Data::Dumper;
use constant {
	GET_NEW_SENSORS =>
	  "SELECT id_sensor,value,description FROM Sensors WHERE auditory=?",
	GET_CUR_VALUE_SENSORS =>
"SELECT value FROM Parametrs WHERE id_sensor=? ORDER BY time DESC LIMIT 1",
	VALUE_NOT_FOUND => "Датчик не имеет измерений"
};

my $query_cgi = CGI::new();

print "Content-type: text/plain; charset=UTF8\n\n";
print length( $query_cgi->param("id") ) . $query_cgi->param("id");
my $arr = select_sql( GET_NEW_SENSORS, $query_cgi->param("id") );
if ($arr) {
	my @sensors = @{$arr};
	print "<table border='1px' style=\"width: 100%;\">";
	foreach my $sensor (@sensors) {

		my ($val_sen) =
		  exec_sql_fetchrow_array( GET_CUR_VALUE_SENSORS,
			$sensor->{id_sensor} );
		my $desc = $sensor->{description};
		if ( not defined $desc ) {
			$desc = "Описание отсутствует";
		}
		if ( $sensor->{value} eq 'T' ) {
			if ( not defined $val_sen ) {
				$val_sen = VALUE_NOT_FOUND;
			}
			else {
				$val_sen = $val_sen . " °C";
			}
			print "
			  <tr id="
			  . $sensor->{id_sensor}
			  . " class=\"sensorstable\" style=\"font-size: 15px\" onclick=\"get_statistic(id)\"> 
				  <td style=\"display : none;\"> 
				  " . $sensor->{id_sensor} . "
				  </td>
				  <td>" . $desc . "</td>
				   <td>  
				   Значение = $val_sen 
				  </td>
			  </tr>";
		}
		elsif ( $sensor->{value} eq 'D' ) {
			print "
			  <tr id="
			  . $sensor->{id_sensor}
			  . " class=\"sensorstable\" style=\"font-size: 15px\" onclick=\"get_statistic(id)\"> 
				  <td style=\"display : none;\"> 
				  " . $sensor->{id_sensor} . "
				  </td>
				  <td>" . $desc . "</td>";

			if ( not defined $val_sen ) {
				print "<td >" . "ХЗ" . "</td>";
			}
			elsif ($val_sen == 0) {
				print "<td style=\"background-color: #71c73e;\">"
				  . "Открыто" . "</td>";
			}
			elsif ($val_sen == 1) {
				print "<td style=\"background-color: #FF1010;\">"
				  . "Закрыто" . "</td>";
			}
			print "</td></tr>";
		}
	}
	print "</table>";
}

