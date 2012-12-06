#!c:/perl/bin/perl.exe
use strict;
use CGI qw(:all escape);
use Database;

use constant {
	GET_VALUES_SENSORS1 =>
	  "SELECT * FROM Parametrs WHERE id_sensor=? and time>=",
	GET_VALUES_SENSORS2 => " and time<=",
	GET_VALUES_SENSORS3 => " ORDER BY time DESC limit 500;"
};

my $query_cgi = CGI::new();

print "Content-type: text/plain; charset=UTF8\n\n";
my ( $str1 ) = split( "%20", $query_cgi->param('st') );
my ( $str3 ) = split( "%20", $query_cgi->param('fi') );

my $query =
    GET_VALUES_SENSORS1
  . "DATE_ADD(\"$str1\", INTERVAL 0 SECOND)"
  . GET_VALUES_SENSORS2
  . "DATE_ADD(\"$str3\", INTERVAL 0 SECOND)"
  . GET_VALUES_SENSORS3;

my $arr = select_sql(
	$query,
	$query_cgi->param("id")
);

if ($arr) {
	my @sensors = @{$arr};
	print "<table border='1px' style=\"width: 100%;\">";
	foreach my $sensor (@sensors) {

		my $val_sen = $sensor->{value};
		my $desc    = $sensor->{description};
		if ( not defined $desc ) {
			$desc = "Описание отсутствует";
		}
		print "
			  <tr id="
		  . $sensor->{id_sensor}
		  . " class=\"sensorstable\" style=\"font-size: 15px\"> 
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
		else {
			print "<td >" . " хз " . "</td>";
		}

		print "<td>" . $sensor->{time} . "</td>";
		print "</td></tr>";
	}

	print "</table>";
}
else {
	print
"Состояние в этот период времени не менялось";
}

