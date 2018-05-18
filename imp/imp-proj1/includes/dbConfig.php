<!--
 * Faculty of Information Technology at Brno University of Technology
 * Subject: IMP - Microprocessors and Embedded Systems
 * Topic:  Temperature measuring using DS18B20 and ESP8266 (IoT, cloud)
 * Date:  December 2017
 * Author: Jan Kubica (xkubic39@stud.fit.vutbr.cz)
-->

<?php

date_default_timezone_set('CET');

function connectDB(){
        define("DB_HOST","localhost");
        define("DB_USER","root");
        define("DB_PASS","kubica");
        define("DB_DB","IMP");

        $db = mysqli_connect(DB_HOST, DB_USER, DB_PASS,DB_DB);
        if (!$db) {
            echo "Nepodařilo se připojit k MySQL databázi. <br>";
            die(mysqli_connect_error());
        }
        return $db;
}

$conn = connectDB();

?>