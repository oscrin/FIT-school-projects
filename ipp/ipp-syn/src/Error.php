<?php
/* Faculty of Information Technology, Brno University of Technology
*  IPP (Principles of Programming Languages) - Project 1 - SYN version
*  Date created: February 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: Error.php - Module for printing error messages.
*/

function error( $errNum , $msg ) {
    fwrite(STDERR, $msg . "\n");
    exit($errNum);
}

?>
