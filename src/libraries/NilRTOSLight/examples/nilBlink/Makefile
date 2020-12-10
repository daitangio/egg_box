# Arduino IDE: 1.5.x
ARDUINO_DIR =  ../../../../../arduino-1.8.13
# We also need extra flags -DEXTRA_CPP_FEATURE -std=c++11 -std=gnu++11 

BOARD_TAG= mega
BOARD_SUB= atmega2560


USER_LIB_PATH= ../../../../libraries
ARDUINO_LIBS = NilRTOSLight
 

# add -DDEBUG -DDEBUG_LOG  for extra info
# See https://gcc.gnu.org/gcc-7/changes.html for gcc 7.3.0 enhancements
CPPFLAGS += -DEXTRA_CPP_FEATURE  -felide-constructors  -std=c++0x -Wshadow=global  -Wnonnull

include ../../../../../Arduino-Makefile-master/Arduino.mk

# For emacs fly-make
check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)

