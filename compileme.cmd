@echo off
REM This script show how to compile via command line and dump the data.
REM VERBOSE MODE STILL DOES NOT WORK
REM See https://github.com/arduino/Arduino/blob/ide-1.5.x/build/shared/manpage.adoc
REM for more information
REM ARDUINO_HOME Needs to be set to ARDUINO 1.5.X IDE
SET A_IDE=C:\giorgi\arduino-1.5.8
SET PATH=%PATH%;%A_IDE%\
REM set nodosfilewarning=true

REM VERBOSE MODE DID NOT WORK
%A_IDE%\arduino  --pref build.path=c:/tmp    --verify --board arduino:avr:uno  ./src/the_4egg_box_v2_rtos/the_4egg_box_v2_rtos.ino
@echo off
echo "Dumping Assembly...."
@echo on
REM the_4egg_box_v2_rtos.cpp.elf
%A_IDE%\.\hardware\tools\avr\bin\avr-objdump.exe  -S /cygdrive/c/tmp/*.cpp.elf >sketch.asm