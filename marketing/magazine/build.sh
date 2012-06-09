#!/bin/bash
# Name: build.sh
#
# Builds the magazine article.
# 
# Usage: build.sh [ps,pdf]

set -e # bash should exit the script if any statement returns a non-true 
       #return value
mainfile=main.tex

# Create the output file names
dvifile="$(echo "$mainfile"|sed 's/\..*/.dvi/')"
psfile="$(echo "$mainfile"|sed 's/\..*/.ps/')"

# Create the code snippet figure
echo "* Creating code snippet figure"
cd figs
snipfig="command_array.fig"
snipeps="$(echo "$snipfig"|sed 's/\..*/.eps/')"
echo $snipeps
fig2dev -L eps -m 1 "$snipfig" "$snipeps" # Convert to eps
./bboxfix.sh "$snipeps" temp # Fix the bounding box
mv temp "$snipeps"
cd ..
echo "* Done with code snippet figure"
echo "----------------------------------------------------------------"



# Run latex on the main document file to create the dvi file.  You may
# have to do this a bunch of times to settle latex output.
latex $mainfile -output-format=dvi
echo "* Done with latex output"
echo "----------------------------------------------------------------"


if [ "$1" = "ps" ]
then
    echo "* Creating ""$psfile"
    dvips -t letter "$dvifile"
    echo "* Done with dvips output"
    echo "----------------------------------------------------------------"
fi

