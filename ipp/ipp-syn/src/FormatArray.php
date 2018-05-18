<?php
/* Faculty of Information Technology, Brno University of Technology
*  IPP (Principles of Programming Languages) - Project 1 - SYN version
*  Date created: March 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: FormatArray.php - Module for formating input file.
*/

require 'Error.php';

class FormatArray {
    public $formatArr;  // InputFile jako pole
    public $formatFile; // vicerozmerne pole FormatFile
    public $inputFile;  // InputFile jako string
    public $inputLen;   // delka InputFile ve znacich
    public $outputFile; // zpracovany InputFile s tagy jako string

    function __construct($file) {
        $this->inputFile = $file;
        $this->inputLen = strlen($file);
        $this->formatArr = str_split($file);
    }

    function loadFormatFile($file) {
        $this->formatFile = $file;
    }

    function loadFormatTags() {
        $formatValues = array("bold","italic","underline","teletype");                  // mozne hodnoty formatovacich prikazu
        for ($n = 0; $n < (count($this->formatFile)); $n++) {                           // prochazeni radku FormatFile
            $formatCommands = preg_split("/[ \t]*,[ \t]*/", $this->formatFile[$n][1]);  // pole formatovacich prikazu jednoho radku
            $formatCommands = array_reverse($formatCommands);                           // obraceni sledu prikazu
            
            $countFormatCommands = array_count_values($formatCommands);                 // vyhodnoceni poctu stejnych prikazu
            $cacheArr = array();                                                        // pomocne pole
            /* ------------------ KONTROLA PLATNYCH FORMATOVACICH PRIKAZU ------------------- */
            foreach ($formatCommands as $formatCommand) {
                if (in_array($formatCommand, $formatValues)) {
                    if ($countFormatCommands[$formatCommand] > 1)
                        error(4, "Multiple format command: " . $formatCommand);
                    continue;
                } else if (preg_match("/size:(1|2|3|4|5|6|7)/", $formatCommand) === 1) {
                    if (preg_match_all("/size:(1|2|3|4|5|6|7)/", $this->formatFile[$n][1], $cacheArr) > 1)
                        error(4, "Multiple size command in format file!");
                    continue;
                } else if (preg_match("/color:[0-9A-Fa-f]{6}/", $formatCommand) === 1) {
                    if (preg_match_all("/color:[0-9A-Fa-f]{6}/", $this->formatFile[$n][1], $cacheArr) > 1)
                        error(4, "Multiple color command in format file!");
                    continue;
                } else {
                    if ($formatCommand !== "")
                        error(4, "Unsupported format command: " . $formatCommand);
                    else
                        error(4, "Comma error in format file!");
                }
            }
            /* ------------------ VYKONAVANI FORMATOVACICH PRIKAZU -------------------- */
            foreach ($formatCommands as $formatCommand) {
                $b = preg_match("/bold/", $formatCommand ) === 1 ? true : false;
                $i = preg_match("/italic/", $formatCommand ) === 1 ? true : false;
                $u = preg_match("/underline/", $formatCommand ) === 1 ? true : false;
                $t = preg_match("/teletype/", $formatCommand ) === 1 ? true : false;
                $c = preg_match("/color:[0-9A-Fa-f]{6}/", $formatCommand, $color ) === 1 ? true : false;
                if ($c === true) {
                    $color = implode('', $color);
                    $color = substr($color, 6);
                }
                $s = preg_match("/size:(1|2|3|4|5|6|7)/", $formatCommand, $size ) === 1 ? true : false;
                if ($s === true) {
                    $size = implode('', $size);
                    $size = substr($size, 5, 1);
                }
                $this->formatFile[$n][0] = preg_replace("/\//", "\/", $this->formatFile[$n][0]); // pro <\> tagy ve vstupnim textu

                $h = preg_match_all("/" . $this->formatFile[$n][0] . "/", $this->inputFile, $matches, PREG_OFFSET_CAPTURE); // hledani indexu podle regexu v input file (string)
                if ($h !== 0) {
                /* ---------------- PRIDAVANI FORMATOVACICH TAGU DO POLE VSTUPNIHO SOUBORU -------------------- */
                    for ($j = 0; $j < count($matches[0]); $j++) {
                        $str = strlen($matches[0][$j][0]);
                        $begin = $matches[0][$j][1];
                        $end = $begin + $str - 1;
                        if ($begin > $end) {
                            continue; // redukce prazdneho retezce
                        }
                        if ($b == true) {
                            $this->formatArr[$begin] = '<b>' .  $this->formatArr[$begin];
                            $this->formatArr[$end] = $this->formatArr[$end] . '</b>';
                        }
                        if ($i == true) {
                            $this->formatArr[$begin] = '<i>' .  $this->formatArr[$begin];
                            $this->formatArr[$end] = $this->formatArr[$end] . '</i>';
                        }
                        if ($u == true) {
                            $this->formatArr[$begin] = '<u>' .  $this->formatArr[$begin];
                            $this->formatArr[$end] = $this->formatArr[$end] . '</u>';
                        }
                        if ($t == true) {
                            $this->formatArr[$begin] = "<tt>" .  $this->formatArr[$begin];
                            $this->formatArr[$end] = $this->formatArr[$end] . '</tt>';
                        }
                        if ($c == true) {
                            $this->formatArr[$begin] = "<font color=#" . $color . ">" .  $this->formatArr[$begin];
                            $this->formatArr[$end] = $this->formatArr[$end] . '</font>';
                        }
                        if ($s == true) {
                            $this->formatArr[$begin] = "<font size=" . $size . ">" .  $this->formatArr[$begin];
                            $this->formatArr[$end] = $this->formatArr[$end] . '</font>';
                        }                
                    } // for matches
                } // if ($h !== 0)
                $b = $u = $i = $t = $c = $s = false; // resetovani nastavovacich hodnot
            } // foreach command
        } // for radek FormatFile
        $this->outputFile = implode("",$this->formatArr);
    }

    function addBr() {
        $this->outputFile = preg_replace("/\n/", "<br />\n", $this->outputFile);                
    }

    function makeOutputFile() {
        return $this->outputFile;
    }

}

?>