# Getting Started

- A Linux / macOS environment is easier to setup, because we use command line build (ArduinoMakefile project) during development.
  Refer to codeSpaceSetup.sh script to setup an arduino command line for x86
- Install Arduino Legacy 1.8.13 IDE from https://www.arduino.cc/en/software
- Clone project with recurse submodule: git clone --recurse-submodules

## First project to run: the_4egg_box_v3

The 4egg box is a flashing and sound toy, with no input.
It uses a real-time operating system (NilRTOS) to provide sound and lights effects

### Wiring
Refer to the 'WIRING' section at the start of the sketch

### Compile and run

    cd the_4egg_box_v3
    export ARDUINO_DIR=../../arduino-1.8.13
    make

To upload, plug-in an Arduino uno and type

    make upload

### More advanced project: .... TBD
A more "





---
ISR() is a standard embedded c method for calling an interrupt vector for atmega.
https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
