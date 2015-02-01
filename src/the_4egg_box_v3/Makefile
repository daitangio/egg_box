# Make file based on project
# https://github.com/sudar/Arduino-Makefile
# Version:
# d535bf5fde2d49b190913e858ce644c1a50c3c40
# Fri Jan 23 15:40:53 2015 +0530
# Arduino IDE: 1.5.x
# We also need extra flags -DEXTRA_CPP_FEATURE -std=c++11 -std=gnu++11 
BOARD_TAG    = uno
# Separated by spaces
ARDUINO_LIBS = ../libraries/egg_utils ../libraries/NilRTOS

# add -DDEBUG -DDEBUG_LOG  for extra info
CPPFLAGS += -DEXTRA_CPP_FEATURE  -felide-constructors  -std=c++0x

include ../../../Arduino-Makefile/Arduino.mk

# For emacs fly-make
check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)

doxygen:
	doxygen Doxyfile
