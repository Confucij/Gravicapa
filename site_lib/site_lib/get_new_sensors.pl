#!c:/perl/bin/perl.exe

use strict;
use CGI qw(:all escape);
use Database;
use Data::Dumper;
use constant {
	GET_NEW_SENSORS => "SELECT id_sensor,value FROM Sensors WHERE auditory=0",
	GET_CUR_VALUE_SENSORS =>
"SELECT value FROM Parametrs WHERE id_sensor=? ORDER BY time DESC LIMIT 1",
	VALUE_NOT_FOUND => "Датчик не имеет измерений"
};

print "Content-type: text/plain; charset=UTF8\n\n";

my $arr = select_sql(GET_NEW_SENSORS);
if ($arr) {
	my @sensors = @{$arr};
	print "<table border='1px' style=\" width: 100%; height: 100%; \">";
	foreach my $sensor (@sensors) {
		my ($val_sen) =
		  exec_sql_fetchrow_array( GET_CUR_VALUE_SENSORS,
			$sensor->{id_sensor} );
		if ( not defined $val_sen ) {
			$val_sen = VALUE_NOT_FOUND;
		}
		else {

			#	$val_sen = $val_sen . " °C";
		}
		print "
			  <tr onclick=\"setsendial_open(event,id)\" id="
		  . $sensor->{id_sensor}
		  . " class=\"sensorstable\" style=\"font-size: 12px\"> 
				  <td> 
				  Датчик №" . $sensor->{id_sensor} . "
				  </td>
				   <td>  
				   Значение=$val_sen 
				  </td>
			  </tr>";

	}
	print "</table>";
}
else {
	print "New Sensors not found. \n
	It's good =)";
}

