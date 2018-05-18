#!/usr/bin/env bash

# Project: ISJ-proj1
# Author:  Jan Kubica (xkubic39)
# E-mail:  xkubic39@stud.fit.vutbr.cz
# Date:    3.2.2016

# INPUT: xkcd1313.ipynb (defaultly)
# OUTPUT: xkubic39.ipynb

# This is simple script for text modification using sed.

# Set "FILENAME" variable according to 1st arg (defaultly to "xkcd1313.ipynb")
FILENAME=${1:-"xkcd1313.ipynb"} 

if [ "$#" -gt 1 ]; then
		echo "Script doesn't support more than 1 argument [FILENAME]."
elif [ -e $FILENAME ]; then
		sed -e '/xkcd/ s/bu.*ls/\[gikuj\]\.\.n\|a\.\[alt\]\|\[pivo\]\.l\|i\.\.o\|\[jocy\]e\|sh\|di\|oo/g' -e '/\[/ {/=/ {/!/! s/xkcd/xkubic39/g}}' $FILENAME > xkubic39.ipynb
else
		echo "Input file not found."
		exit 1
fi
