#!/bin/bash
# Name: termgrab.sh
#
# Grabs a terminal window and creates the image file "grab.eps"
# Uses the "import" tool provided by Imagemagick
#
# Usage: termgrab.sh
# ...then click in the window you want to grab with the crosshairs.
set -e # bash should exit the script if any statement returns a non-true 
       #return value
echo "Use the crosshair to select the window to grab."
# Options for import:
# -frame -- Include the window frame with the window grab.
# -density -- Image density in pixels per inch (default)
import -frame -density 300x300 grab.tif
# Options for convert:
# -resample XRESxYRES -- set the image resolution
# -negate -- Invert each pixel color
convert -resample 50x50 \
	grab.tif grab.eps
echo "Window image saved to grab.eps and grab.tif."
