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

#### How it works
The project used the midicsv program to convert midis to a set of static files.
To install midicsv on Linux, issue

    apt install midicsv

On MacOs try

    brew install midicsv

Homepage: http://www.fourmilab.ch/webtools/midicsv/
To convert a midi try something like




## How it works
The program uses two competing threads on the same led group: the first play the music, the seconds fade the led in a cyclic way.

The resulting effect is like a strobo-system: leds fade in and out when no sound is in emitted, but pulse at rythm based on the note played.


### The more advanced project: mega_lighting_box



### More advanced project: tnt_box_2023 

Feature:
    - A 10x10cm box with a Display, a count down button and lights effect
    - This version uses a simpler approach than egg box and enable deep sleep for a always-on installation.


Refer to [specific readme](./src/tnt_box_2023/README_tnt_box.md) which include detailed build instructions

### About ChNil from Greiman [7th October 2023]

We tested  https://github.com/greiman/ChNil,
revision f37322d7607bd38fc6804c43e98f68882f9868f7 from 2 Aug 2017
to refresh our NilRTOS library

The code is Nil version 2.0.0 from Stable Version 17.6 of ChibiOS.

This code should be better than NilRTOS but:

- The critical code (context switch) is the same of NilRTOS
- No big advantage seen so far
- The unused stack size function report wrong value(0)

so we decided to put it aside for the meantime.


### Other projects



---
ISR() is a standard embedded c method for calling an interrupt vector for atmega.
https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
