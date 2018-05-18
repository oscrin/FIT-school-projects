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
<title>Historie teploty</title>
</head>
<body>

<div>
<h2>Historie teplotních údajů</h2>
</div>

<table>
        <tr>
                <td> <a href="main.php">Zpět</a> </td>
        </tr>
</table>

<br>

<div style="position: absolute; top: 10px; right: 10px; font-size: 90%; color: #333;">
        ©Jan Kubica, email:<a href="mailto:xkubic39@stud.fit.vutbr.cz" style="text-decoration: none; color: #555;">xkubic39@stud.fit.vutbr.cz</a>
</div>

<?php

$query = "SELECT temp_date, temp_sensor, temp_value FROM Temperature ORDER BY temp_date DESC";

mysqli_query($conn, $query) or die('Error querying database.');

$result = mysqli_query($conn, $query);

echo "<table border='1'>";
echo "<tr> <th> Teplota </th> <th> Datum </th> <th> Senzor </th> <tr>";
while ($row = mysqli_fetch_array($result)) {
echo "    
    <tr>
	    <td style='width: 180px;'> Aktuální teplota: <strong>" . $row['temp_value'] . "</strong> °C   </td> <td style='width: 170px; text-align: center;'> <i>(" . $row['temp_date'] .")</i> </td> <td style='width: 30px; text-align: center;'> <i> (" . $row['temp_sensor'] .")</i> </td>
    </tr>
	";
}
echo "</table>";

$conn->close();

?>

</body>
</html>