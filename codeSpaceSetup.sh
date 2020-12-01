#!/bin/bash

#Dowload arduino-1.8.13
if [ ! -f ./arduino-1.8.13-linux64.tar.xz ] ; then
 curl -LO https://downloads.arduino.cc/arduino-1.8.13-linux64.tar.xz
fi
if [ ! -d  arduino-1.8.13 ]; then
    echo "Expanding arduino..."
    tar xf arduino-*-linux64.tar.xz
fi
