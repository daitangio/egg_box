#!/bin/bash
set -o errexit
set -o nounset
# Command line is still quite bugged at least under MS-Windows7
# +verbose mode did not work
# +launching from a directory different from $ARDUINO_HOME gets strange error about unable to find sketch
#   and/or cygwin*.dll missed error,
# See https://github.com/arduino/Arduino/blob/ide-1.5.x/build/shared/manpage.adoc
export ARDUINO_HOME=$HOME/arduino-1.5.8
export EGG_BOX=$HOME/egg_box
export BUILD_PATH=$EGG_BOX/build
mkdir -p $BUILD_PATH
cd $ARDUINO_HOME
echo Issuing compilation 
time ./arduino  --pref build.path=$BUILD_PATH    --verify --board arduino:avr:uno  $EGG_BOX/./src/the_4egg_box_v2_rtos/the_4egg_box_v2_rtos.ino
echo "Dumping Assembly...."
./hardware/tools/avr/bin/avr-objdump.exe  -S $BUILD_PATH/*.cpp.elf > $EGG_BOX/sketch.asm
cd $EGG_BOX
