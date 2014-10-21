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
echo Patching....
# Only GCC 4.7 and later support -std=c++11 .... -std=gnu++0x 
echo "compiler.cpp.extra_flags=-DEXTRA_CPP_FEATURE  -std=c++11 -std=gnu++11 " >$ARDUINO_HOME/hardware/arduino/avr/platform.local.txt

echo Issuing compilation 
# extra_flags did not work!
time ./arduino  --pref build.path=$BUILD_PATH    --verify --board arduino:avr:uno  $EGG_BOX/./src/lambda_test/lambda_test.ino
echo "Compilation went well"
echo "Dumping Assembly...."
./hardware/tools/avr/bin/avr-objdump.exe  -S $BUILD_PATH/*.cpp.elf > $EGG_BOX/sketch.asm
cd $EGG_BOX
