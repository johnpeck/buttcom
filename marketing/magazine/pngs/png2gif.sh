#!/bin/bash
# Name: png2gif.sh
#
# Converts a png file to a gif using the convert utility provided by
# Imagemagick
#
# Usage: png2gif.sh <input png file>
# ...then click in the window you want to grab with the crosshairs.

set -e # bash should exit the script if any statement returns a non-true 
       #return value
if [ -z "$1" ]
then
    echo "Usage: png2gif.sh <png file>"
else
    giffile="$(echo $1|sed 's/\..\{3\}$/.gif/')"
    # Options for convert:
    # -resample XRESxYRES -- set the image resolution
    # -negate -- Invert each pixel color
    convert "$1" "$giffile"
    echo "$1" "--->" "$giffile"
fi
