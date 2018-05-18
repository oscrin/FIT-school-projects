#!/bin/sh

# Project: IOS-proj1
# Author:  Jan Kubica (xkubic39)
# E-mail:  xkubic39@stud.fit.vutbr.cz
# Date:    20.3.2016

# ----BEGIN OF SCRIPT------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# set sorting localization to POSIX defaultly
# LANG=POSIX
# LC_CTYPE="POSIX"
LC_NUMERIC="POSIX"
LC_TIME="POSIX"
LC_COLLATE="POSIX"
LC_MONETARY="POSIX"
LC_MESSAGES="POSIX"
LC_PAPER="POSIX"
LC_NAME="POSIX"
LC_ADDRESS="POSIX"
LC_TELEPHONE="POSIX"
LC_MEASUREMENT="POSIX"
LC_IDENTIFICATION="POSIX"

export LC_NUMERIC="POSIX"
export LC_TIME="POSIX"
export LC_COLLATE="POSIX"
export LC_MONETARY="POSIX"
export LC_MESSAGES="POSIX"
export LC_PAPER="POSIX"
export LC_NAME="POSIX"
export LC_ADDRESS="POSIX"
export LC_TELEPHONE="POSIX"
export LC_MEASUREMENT="POSIX"
export LC_IDENTIFICATION="POSIX"

# ----ARGUMENTS AND CONDITIONS---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# set root directory according to 1st arg (defaultly to "pwd")
DIR=${1:-`pwd`}

# if 1st argument = "-i", 2nd argument has to be regex specifier of discarded file or directory
# 3rd argument set to "pwd" defautly or different root directory manually
if [ "$1" = "-i" ]
then
	[ -z "$2" ] && echo 1>&2 "ERROR: No \"FILE_ERE\" argument after -i" && exit 1
	FILE_ERE="$2"
	DIR=${3:-`pwd`}
fi

if [ "$#" -gt 3 ]; then
    echo 1>&2 "ERROR: Illegal number of arguments"
    exit 1
fi

if [ "$2" = "^" -o "$2" = "$" -o "$2" = "."  -o "$2" = ".*" -o "$2" = "/" -o "$2" = "/." -o "$2" = "./" ]
then
	echo 1>&2 "ERROR: Regex input error"
	exit 1
fi

# used variables error
case $FILE_ERE in 
  *%*)
    echo 1>&2 "ERROR: \"%\" not supported in this script"
	exit 1
	;;
  *°*)
    echo 1>&2 "ERROR: \"°\" not supported in this script"
    exit 1
    ;;
esac

# ----FILE_ERE AND CONDITIONS----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# FILE_ERE makes condition for files
FILE_ERE=`echo $FILE_ERE | sed -e "s/^[\ \ ]/%/g"`
FILE_ERE=`echo $FILE_ERE | sed -e "s/\^/\\\\//g"`

# FILE_ERE makes condition for directories
FILE_ERE_2=`echo $FILE_ERE | sed -e "s/[$]/\\\\//g"`

# if no regular expresion is given
if [ -z "$FILE_ERE" ]; then
	FILE_ERE="°"
fi

if [ -z "$FILE_ERE_2" ]; then
	FILE_ERE_2="°"
fi
# ----ROOT DIRECTORY AND VALIDATION----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# validation of existing root direcory
if [ -d $DIR ]
then
	true
else
	echo 1>&2 "ERROR: Root path not found"
	exit 1
fi

# prints the root directory
echo "Root directory:" $DIR

# removes final "/" in the end from path
DIR=`echo $DIR | sed 's/\/$//'`

# modification of root directory if no path given 
# ( makes from absolute path relative to get work with regular expresions ) 
DIR_PW=`pwd`
if [ $DIR = $DIR_PW ]
then
	DIR="."
	DIR_COUNT="0"
else
	DIR_COUNT=`echo $DIR | sed 's/^[./]*\//\//g' | sed 's/[^/]//g' | awk '{ print length; }'`
fi

# ----NUMBER OF DIRECTORIES AND VALIDATION---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# number of directories in root directoryS
ND=`find $DIR -type d -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | sed '1d' | egrep -v "$FILE_ERE" | egrep -v "$FILE_ERE_2" | wc -l`
ND=$(((ND) + 1))


# prints number of contained directories
echo "Directories:" $ND

# ----MAXIMAL DEPTH--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# Maximal depth of directories in root directory
DD=`find $DIR -type d -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | sed '1d' | egrep -v "$FILE_ERE" | egrep -v "$FILE_ERE_2" | sed 's/[^/]//g' | awk '{ print length; }' | sort -n -r | head -1`
DD=$(((DD) + 1))

# prints maximal depth of directories in root directoryls
echo "Max depth:" $DD

# ----NUMBER OF FILES------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# number of files in root directory
NF=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11 }' | sed 's/\\\ /%/g' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v "$FILE_ERE" | egrep -v "$FILE_ERE_2" | wc -l`

# if [ $NF -eq 0 ]
# then
# 	echo 1>&2 "ERROR: Regex argument covers all the files in root directory"
# 	exit 1
# fi

# ----AVERAGE = NO. OF FILES/ NO. DIRECTORIES------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# count of average number of files in directory
AF=$((NF / ND))

# prints average no. of files in directory
echo "Average no. of files:" $AF

# ----NUMBER OF FILES------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# prints amount of files
echo "All files:" $NF

# ----LARGEST FILE---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# largest file in root directory, "N/A" if 0 number of files
if [ "$NF" -eq "0" ]
then
	LF="N/A"
else
LF=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | awk -F' ' '{ print $2 }' | sort -r -n | head -1`
	if [ "$LF" -eq "0" ]
	then
		LF="N/A"
	fi
fi

# prints size of the largest file in bytes
echo "  Largest file:" $LF

# ----AVERAGE FILE SIZE----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# average size of file, "N/A" if 0 number of files
if [ "$NF" -eq "0" ]
then
	AS="N/A"
else
	AS=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | awk '{sum += $2; n++;} END {print int(sum/n);}'`
fi

# average file size
echo "  Average file size:" $AS

# ----SIZE MEDIAN----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# finds size median of all files, "N/A" if 0 number of files
if [ "$NF" -eq "0" ]
then
	MS="N/A"
else
	H=$((NF % 2))
	if [ $H -eq 1 ]
	then
		MS_P=$(((NF / 2) + 1))
	    MS=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | awk -F' ' '{ print $2 }' | sort -r -n | sed -n "$MS_P p"`
	elif [ $NF -eq 1 ]
	then
		MS=$LF
	else
		MS_P1=$((NF / 2))
		MS_P2=$(((NF / 2) + 1))
		MS1=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | awk -F' ' '{ print $2 }' | sort -r -n | sed -n "$MS_P1 p"`
		MS2=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | awk -F' ' '{ print $2 }' | sort -r -n | sed -n "$MS_P2 p"`
		MS=$(((MS1 + MS2 ) / 2))
	fi
fi

# file size median
echo "  File size median:" $MS

# ----FILE EXTENSIONS------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ #

# finding file extensions
if [ "$NF" -eq "0" ]
then
	EL=""
else
EL=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v "$FILE_ERE" | egrep -v "$FILE_ERE_2" | sed 's/^.*[^\/]\.//g' | sed 's/.*\/.*//g' | sed '/[^a-zA-Z0-9]/ s/.*//g' | sort -u -V | sed "/^$/d" | awk '{printf "%s,", $0} END {printf "\n"}' | sed 's/,$//g'`
fi

# comma-separated extension list
echo "File extensions:" $EL

# ----EXTENSIONS CYCLE------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ #

# cycle for finding file extensions details
COUNT_LIMIT=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v "$FILE_ERE" | egrep -v "$FILE_ERE_2" | sed 's/^.*[^\/]\.//g' | sed 's/.*\/.*//g' | sed '/[^a-zA-Z0-9]/ s/.*//g' | sort -u -V | sed "/^$/d" | wc -l`
COUNT=1

while [ $COUNT -le $COUNT_LIMIT ]
do

# ----NAME OF EXTENSION----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# name of extension
EXT=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v "$FILE_ERE" | egrep -v "$FILE_ERE_2" | sed 's/^.*[^\/]\.//g' | sed 's/.*\/.*//g' | sed '/[^a-zA-Z0-9]/ s/.*//g' | sort -u -V | sed "/^$/d" | sed -n "$COUNT p"`

# ----NUMBER OF EXTENSION FILES--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# number of files with specified extension
NEXT=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v "$FILE_ERE" | egrep -v "$FILE_ERE_2" | grep "[^/]\.$EXT$" | wc -l`
echo "Files .$EXT:" $NEXT

# ----MAXIMAL SIZE OF EXTENSION FILES--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# maximal file size of files with specified extension
LEXT=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | grep "[^/]*\.$EXT .*" | awk -F' ' '{ print $2 }' | sort -r -n | head -1`
echo "  Largest file .$EXT:" $LEXT

# ----AVERAGE SIZE OF EXTENSION FILES--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# average file size of files with specified extension
AEXT=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | grep "[^/]*\.$EXT .*" | awk '{sum += $2; n++;} END {print int(sum/n);}'`
echo "  Average file size .$EXT:" $AEXT

# ----SIZE MEDIAN OF EXTENSION FILES---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #

# file size median of files with specified extension ()
C=$((NEXT % 2))
if [ $C -eq 1 ]
then
	MEXT_P=$(((NEXT / 2) + 1))
    MEXT=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | grep "[^/]*\.$EXT .*" | awk -F' ' '{ print $2 }' | sort -r -n | sed -n "$MEXT_P p"`
elif [ $NEXT -eq 1 ]; then
	MEXT=$LEXT
else
	MEXT_P1=$((NEXT / 2))
	MEXT_P2=$(((NEXT / 2) + 1))
	MEXT1=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | grep "[^/]*\.$EXT .*" | awk -F' ' '{ print $2 }' | sort -r -n | sed -n "$MEXT_P1 p"`
	MEXT2=`find $DIR -type f -ls | sed 's/\\\ /%/g' | awk -F' ' '{ print $11, $7 }' | sed 's/^[./]*\//\//g' | awk -v var="$DIR_COUNT" '{ for (i = var; i > 0; --i) { sub("^/[^/]*/", "/")}} {print}' | egrep -v ".*$FILE_ERE.* .*" | egrep -v ".*$FILE_ERE_2.* .*" | grep "[^/]*\.$EXT .*" | awk -F' ' '{ print $2 }' | sort -r -n | sed -n "$MEXT_P2 p"`
	MEXT=$(((MEXT1 + MEXT2) / 2))
fi

echo "  File size median .$EXT:" $MEXT

COUNT=$((COUNT + 1))
done

# ----END OF SCRIPT-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- #