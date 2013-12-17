#!/bin/bash
# Convert images to PNG/JPG.
# Note: ImageMagick is bad at writing PNG at times.
# JPG always works.

for f in $(ls *.ppm); do
    t=$(echo $f | sed 's/\.ppm/\.jpg'/)
    echo $t
    convert $f $t
    rm $f
done
