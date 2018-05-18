<?php
/* Faculty of Information Technology, Brno University of Technology
*  IPP (Principles of Programming Languages) - Project 1 - SYN version
*  Date created: March 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: RegexConverter.php - Module for modifying regular expression from format file.
*/

function regexConv($reg) {

    $regArr = array();                      // pole regexu
    $strLen = mb_strlen($reg, 'UTF-8');     // delka regexu ve znacich v UTF-8
    for ($j = 0; $j < $strLen; $j++) {      // rozdeleni regexu do pole po jednotlivych znacich
        $regArr[$j] = mb_substr($reg, $j, 1, 'UTF-8');
    }
    
    $neg = ""; // negace v regexu
    $bracket = 0; //  pocet zavorek v regexu (inkrement, dekrement) 
    $finReg = $reg; // vlozeni regexu

    for ($z = 1; $z < $strLen; $z++) {      // odstraneni konkatenaci
        if (($regArr[$z-1] !== "%") && ($regArr[$z] === "."))
            $finReg = substr_replace($finReg, "\037", -$strLen+$z, 1);
    }  
    for ($i = 0; $i < $strLen; $i++) {
        /* -------- POCITANI ZAVOREK --------- */
    //    var_dump($finReg);
        if ($regArr[$i] === "(") {
            if (array_key_exists($i-1, $regArr) && $regArr[$i-1] === "%") {
                if (array_key_exists($i-2, $regArr) && $regArr[$i-2] === "%")
                    $bracket++;
            } else {
                $bracket++;
            }
        }
        if ($regArr[$i] === ")") {
            if (array_key_exists($i-1, $regArr) && $regArr[$i-1] === "%") {
                if (array_key_exists($i-2, $regArr) && $regArr[$i-2] === "%")
                    $bracket--;
            } else {
                $bracket--;
            }
        }
        /* ---------------- NEGACE ----------------- */
        if ($regArr[$i] === "!") {
            if ($neg === "") {
                $neg = "^";
                $finReg = substr_replace($finReg, "\037", -$strLen+$i, 1);
                continue;
            } else {
                $finReg = "";
                return false;
            }
        }
        /* -------------- SPECIALNI ZNAKY --------------- */
        if ($regArr[$i] === "%") {
            $i++;
            if (array_key_exists($i, $regArr)) {
                switch ($regArr[$i]) {
                    case "s": $finReg = substr_replace($finReg, "[" . $neg . "\s]", -$strLen+$i-1, 2); break;
                    case "a": $finReg = substr_replace($finReg, "[" . $neg . "\s\S]", -$strLen+$i-1, 2); break;
                    case "d": $finReg = substr_replace($finReg, "[" . $neg . "0-9]", -$strLen+$i-1, 2); break;
                    case "l": $finReg = substr_replace($finReg, "[" . $neg . "a-z]", -$strLen+$i-1, 2); break;
                    case "L": $finReg = substr_replace($finReg, "[" . $neg . "A-Z]", -$strLen+$i-1, 2); break;
                    case "w": $finReg = substr_replace($finReg, "[" . $neg . "a-zA-Z]", -$strLen+$i-1, 2); break;
                    case "W": $finReg = substr_replace($finReg, "[" . $neg . "a-zA-Z0-9]", -$strLen+$i-1, 2); break;
                    case "t": $finReg = substr_replace($finReg, "[" . $neg . "\t]", -$strLen+$i-1, 2); break;
                    case "n": $finReg = substr_replace($finReg, "[" . $neg . "\n]", -$strLen+$i-1, 2); break;
                    case ".": $finReg = substr_replace($finReg, "[" . $neg . ".]", -$strLen+$i-1, 2); break;
                    case "|": $finReg = substr_replace($finReg, "[" . $neg . "|]", -$strLen+$i-1, 2); break;
                    case "!": $finReg = substr_replace($finReg, "[" . $neg . "!]", -$strLen+$i-1, 2); break;
                    case "*": $finReg = substr_replace($finReg, "[" . $neg . "*]", -$strLen+$i-1, 2); break;
                    case "+": $finReg = substr_replace($finReg, "[" . $neg . "+]", -$strLen+$i-1, 2); break;
                    case ")": $finReg = substr_replace($finReg, "[" . $neg . ")]", -$strLen+$i-1, 2); break;
                    case "(": $finReg = substr_replace($finReg, "[" . $neg . "(]", -$strLen+$i-1, 2); break;
                    case "%": $finReg = substr_replace($finReg, "[" . $neg . "%]", -$strLen+$i-1, 2); break;
                    default:  return false;
                }
            } else
                return false;
            $neg = "";
            continue;
        }
        /* ------------ OSETRENI TECKY ------------*/
        if ($regArr[$i] === '.') {
           if (array_key_exists($i+1, $regArr) && $regArr[$i+1] === '*') {
                if (!array_key_exists($i-1, $regArr)) {
                    return false;
                }
                if (array_key_exists($i-1, $regArr) && $regArr[$i-1] !== '%') {
                    return false;
                }
            }
            if (array_key_exists($i-1, $regArr) && array_key_exists($i+1, $regArr)) {
                continue;
            } else {
                return false;
            }
        }
        /* ------------ NEGACE ZNAKU ------------- */
        if ($neg === "^") {
            $finReg = substr_replace($finReg, "[^" . $regArr[$i] . "]", -$strLen+$i, 1);
            $neg = "";
            continue;
        }
        /* ----- KONTROLA ZAVOREK ------*/
        if ($bracket < 0)
            return false;
    }
    /* ---- ZAVERECNE VYHODNOCENI -------- */
    if ($bracket !== 0 || $neg === "^") {
        return false;
    } else {
        $finReg = str_replace("\037", '', $finReg);
        return $finReg;
    }
} /*
$str = 'a.a';
$b = regexConv($str);
if ($b === false) {
    echo "False\n";
} else {
    echo "$b\n";
}
*/
?>
