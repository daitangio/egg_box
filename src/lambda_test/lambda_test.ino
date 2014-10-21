/* GG See https://gcc.gnu.org/projects/cxx0x.html
  and http://permalink.gmane.org/gmane.comp.hardware.arduino.devel/3350
  You need something like
     echo 'compiler.cpp.extra_flags=-DEXTRA_CPP_FEATURE -std=gnu++0x ' >$ARDUINO_HOME/hardware/arduino/avr/platform.local.txt
  to override standard arduino compiler flags and enable C++ experimental features
  You need also to realunch your arduino ide to force reading the new options
*/

#ifndef EXTRA_CPP_FEATURE
#error "You need to create your custom $ARDUINO_HOME/hardware/arduino/avr/platform.local.txt in your ARDUINO 1.5.x IDE"
#endif

const auto x=1;
auto lambdaGuy = [](int i){ return i; };

void setup() {
  // range special syntax:
  int array[5] = { 1, 2, 3, 4, 5 };
  for (int& x : array) {
    x *= lambdaGuy(x)*2;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
