<!--
 * Faculty of Information Technology at Brno University of Technology
 * Subject: IMP - Microprocessors and Embedded Systems
 * Topic:  Temperature measuring using DS18B20 and ESP8266 (IoT, cloud)
 * Date:  December 2017
 * Author: Jan Kubica (xkubic39@stud.fit.vutbr.cz)
-->

<?php

include_once "includes/dbConfig.php";

?>

<!DOCTYPE html>
<html>
	<head>
		<title>Aktuální teplota</title>
		<meta http-equiv="Refresh" content="60">
	</head>
<body>
	<h1>Měření teploty</h1>

<?php

if($_SERVER['REQUEST_METHOD'] == "POST"){
	$data = file_get_contents('php://input');

	$json_data = json_decode($data);

	$temp = $json_data->measuredValue;
	$sensor = $json_data->sensorNumber;

	$sql = "INSERT INTO Temperature (temp_sensor, temp_value)
		VALUES ('" . $sensor . "','" . $temp . "')";
	    
	if(mysqli_query($conn,$sql)) {
		echo "Success.\n";
	} else {
		echo "Not success." . $sql . "<br>" . mysqli_error($conn);
	}
}

$query = "
SELECT * 
FROM (SELECT * FROM Temperature 
      ORDER BY id DESC 
      LIMIT 1) t
ORDER BY id ASC;
";

mysqli_query($conn, $query) or die('Error querying database.');

$result = mysqli_query($conn, $query);

while ($row = mysqli_fetch_array($result)) {
echo "    
    <p>
	    Aktuální teplota: <strong>" . $row['temp_value'] . "</strong> °C  <i> (tep. " . $row['temp_sensor'] . ")</i> <br>
    </p>
	";

$date = new \DateTime();

echo "
    <p>
	Čas aktualizace: <i>" . $row['temp_date'] . ".</i> <br>
    </p>
";
}
?>

<a href="history.php">Přehled historie</a>

<div style="position: absolute; bottom: 10px; font-size: 90%; color: #333;">
	©Jan Kubica, email:<a href="mailto:xkubic39@stud.fit.vutbr.cz" style="text-decoration: none; color: #555;">xkubic39@stud.fit.vutbr.cz</a>
</div>

<?php

mysqli_close($conn);

?>

</body>
</html>
