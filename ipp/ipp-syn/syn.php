<?php
/* Faculty of Information Technology, Brno University of Technology
*  IPP (Principles of Programming Languages) - Project 1 - SYN version
*  Date created: February 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: xkubic39.php - Main file for running the script
*/

require 'src/Arguments.php';
require 'src/RegexConverter.php';
require 'src/FormatArray.php';

/* ------------------------ NAPOVEDA -------------------------- */
function printHelp() {
    echo ( "\033[93m                              Syntax Highlighting\n\033[0m" );
    echo ( "Basic program made for simple text highlighting, adding HTML tags to the input \nfile. " );
    echo ( "Changes are processed according to specific regular expressions included \nin the format file. " );
    echo ( "For more information please read the project documentation.\n\n" );
    echo ( "\e[1mAuthor:\e[0m Jan Kubica, xkubic39@stud.fit.vutbr.cz\n" );
    echo ( "\e[1mDate created:\e[0m February 2017\n\n" );
    echo ( "\e[96mUsage: \e[39mphp syn.php \e[2m--help\e[0m\n" );
    echo ( "       php syn.php \e[2m[--format=filename] [--input=filename] [--output=filename] [--br]\e[0m\n\n" );
    echo ( "\e[96mOptions:\e[39m\n" );
    echo ( "     \e[1m--help \e[0m                - prints help to stdout\n" );
    echo ( "     \e[1m--format=filename \e[0m     - format file, no changes are made if missing\n" );
    echo ( "     \e[1m--input=filename \e[0m      - input file in UTF-8, if missing, stdin is considered\n" );
    echo ( "     \e[1m--output=filename \e[0m     - output file in UTF-8, if missing, stdout is considered\n" );
    echo ( "     \e[1m--br \e[0m                  - adds a <br /> element at the end of each line\n\n" );
}

$inFileContent = ""; // nacteny vstupni soubor (string)
$FFRows = array(); // nacteny formatovaci soubor po radcich

/* funkce pro zpracovani vstupniho souboru, vraci nacteny vstup typu string */
function parseInputFile($args) {
    if (is_readable($args->inputFile)) {
        $inFileContent = file_get_contents($args->inputFile);
    } else {
        error(2, "Input file failure!");
    }
    return $inFileContent;       
}
/* funkce pro zpracovani formatovaciho souboru, vraci nacteny formatovaci soubor jako pole radku */
function parseFormatFile($args) {
    if (is_readable($args->formatFile)) {
        $FFRows = array();
        $handle = fopen($args->formatFile, "r");
        while (($line = fgets($handle)) == true) {
            if ($line != "\n") {
                $line = str_replace("\n", '', $line);
                array_push($FFRows, $line);
            }
        }
        fclose($handle);
        foreach ($FFRows as &$row) {
            $row = preg_split("/[\t]+/u", $row, 2);
        }
        return $FFRows;
    }
}
/* ---------- ZPRACOVANI ARGUMENTU ----------- */
$args = new Arguments($argv);
$opt = $args->parseArg();

if (!$opt)
    error(1, "Console argument Error!");
if ($args->isHelp) {
    printHelp();
    return 0;
}
/* --------- VSTUP / VYSTUP --------- */
if ($args->isInputFile) {
    $inFileContent = parseInputFile($args);
} else { // cteni ze standartniho vstupu
    while ($f = fgets(STDIN)) {
        $inFileContent .= $f;
    }
}
if ($args->isFormatFile) {
    $FFRows = parseFormatFile($args);

    for ($i = 0; $i < count($FFRows); $i++) {
        $FFRows[$i][0] = regexConv($FFRows[$i][0]); // konvertovani IPP regexu do PCRE regexu
        if ($FFRows[$i][0] === false)
            error(4, "Invalid regex in format file!");
    }
}
/* --------- TVORBA VYSTUPNIHO SOUBORU ------------ */
$transform = new FormatArray($inFileContent);
$transform->loadFormatFile($FFRows);
$transform->loadFormatTags();
if ($args->isBr) {
    $transform->addBr();
}
$out = $transform->makeOutputFile();
if ($args->isOutputFile) {
        $succ = file_put_contents($args->outputFile, $out);
        if ($succ == false)
            error(3, "Non writable output file!");
} else
    echo $out;

return 0;

?>
