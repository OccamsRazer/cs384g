#!/bin/bash

echo "Edit this file and change RAY_TRACER appropriately. Then remove the echo and exit lines"
exit 0

i=0
mkdir -p frames
while [ $i -le 11 ]; do
  RAY_TRACER -r 4 -w 1024 -s 5 artifact$i.ray frames/$i.bmp 
  i=$(( $i + 1 ))
done

convert -delay 10 -loop 0 frames/*.bmp output.gif

