# Arduino IDE: 1.5.x
ARDUINO_DIR = /home/jj/pub/arduino-1.8.13
# We also need extra flags -DEXTRA_CPP_FEATURE -std=c++11 -std=gnu++11 

BOARD_TAG= mega
BOARD_SUB= atmega2560


USER_LIB_PATH= ../libraries
ARDUINO_LIBS = egg_utils LiquidCrystal_I2C Wire
 

# add -DDEBUG -DDEBUG_LOG  for extra info
CPPFLAGS += -DEXTRA_CPP_FEATURE  -felide-constructors  -std=c++0x

include ../../Arduino-Makefile-master/Arduino.mk

# For emacs fly-make
check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)

doxygen:
	doxygen Doxyfile
