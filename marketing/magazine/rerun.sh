#!/bin/bash
# Name: rerun.sh
#
# Touches main.tex if main.log indicates that latex needs to be rerun.
# This allows make to be called again to resolve cross references.
# 
# Usage: rerun.sh

set -e # bash should exit the script if any statement returns a non-true 
       #return value
if [ "$(grep -li 'rerun' main.log)" = "main.log" ]; then 
    touch main.tex
    echo "* Run make again to resolve cross references." 
fi
