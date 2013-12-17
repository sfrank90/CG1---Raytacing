#!/bin/bash
convert -resize 50% -delay 10 -loop 0 $* anim.gif
eog anim.gif
