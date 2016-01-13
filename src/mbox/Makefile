# Make file based on project
# https://github.com/sudar/Arduino-Makefile
# Arduino IDE: 1.5.x

#BOARD_TAG    = uno
# For mega we MUST specify board_sub
BOARD_TAG    = mega
BOARD_SUB=atmega2560
# Separated by spaces
ARDUINO_LIBS = ../libraries/egg_utils ../libraries/NilRTOS ../libraries/Narcoleptic

# add -DDEBUG -DDEBUG_LOG  for extra info
# Try out also -std=c++11 -std=gnu++11 
CPPFLAGS += -DEXTRA_CPP_FEATURE  -felide-constructors  -std=c++0x

include ../../../Arduino-Makefile/Arduino.mk

# For emacs fly-make
check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)

doxygen:
	doxygen Doxyfile
tags:
	etags *.ino ../libraries/*/*.{c,h,cpp}
