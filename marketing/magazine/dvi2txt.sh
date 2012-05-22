#!/bin/bash
# Name: dvi2txt.sh
#
# Converts a dvi file generated with latex to plain ASCII text.  Requires
# catdvi (catdvi.sourceforge.net).
# 
# Usage: dvi2txt.sh <dvi file to open>

set -e # bash should exit the script if any statement returns a non-true 
       #return value
tempfile="temp"
if [ -z "$1" ]
then
    echo "Usage: dvi2txt.sh <dvi file>"
else  
    # Create some temporary files with the same base name but
    # different file extensions.  Always quote filename parameters to
    # preserve space characters.
    rawfile="$(echo $1|sed 's/\..\{3\}$/.rtx/')"
    textfile="$(echo $1|sed 's/\..\{3\}$/.txt/')"
    # Use catdvi to create the raw text (rtx) file.
    # Options:
    # -e -- Output encoding.  Use 1 for US-ASCII
    # -U -- Show the unicode number of unknonwn glyphs instead of ?
    catdvi -e 1 -U "$1" > "$rawfile"
    # use sed 's/  */ /g' to remove consecutive spaces
    cat "$rawfile" | sed 's/  */ /g' > "$textfile"
    cp "$textfile" "$tempfile"
    # use sed 's/^ [[:digit:]][[:digit:]]*$//' to get rid of page numbers
    cat "$tempfile" | sed 's/^ [[:digit:]][[:digit:]]*$//' > "$textfile"
    cp "$textfile" "$tempfile"
    # use tr to remove unprintable characters.  These characters will
    # remain:
    # octal 11: tab
    #       12: linefeed
    #       15: carriage return
    #   40-176: all the "good" keyboard characters
    cat "$tempfile" | tr -cd '\11\12\15\40-\176' > "$textfile"
    rm "$tempfile"
    echo "$1" "--->" "$textfile"
fi
