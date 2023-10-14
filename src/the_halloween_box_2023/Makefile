# Arduino IDE: 1.5.x
# We also need extra flags -DEXTRA_CPP_FEATURE -std=c++11 -std=gnu++11 
BOARD_TAG    = uno
# Separated by spaces
USER_LIB_PATH= ../libraries
ARDUINO_LIBS = egg_utils NilRTOS
 
#ARDUINO_LIBS = egg_utils  NilRTOS

# add -DDEBUG -DDEBUG_LOG  for extra info
CPPFLAGS += -DEXTRA_CPP_FEATURE  -felide-constructors  -std=c++0x

include ../../Arduino-Makefile-master/Arduino.mk

# For emacs fly-make
check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)

doxygen:
	doxygen Doxyfile
