ARDUINO_DIR = ../../arduino-1.8.13


BOARD_TAG    = mega
BOARD_SUB= atmega2560

# Separated by spaces
USER_LIB_PATH= ../libraries
ARDUINO_LIBS = egg_utils NilRTOS


# add -DDEBUG -DDEBUG_LOG  for extra info
CPPFLAGS += -DEXTRA_CPP_FEATURE  -felide-constructors  -std=c++0x

include ../../Arduino-Makefile-master/Arduino.mk

# For emacs fly-make
check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)

doxygen:
	doxygen Doxyfile
