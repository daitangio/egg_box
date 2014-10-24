/* Example of new C++ 11 Features
   C++11 has been finalized around 2013, so I think we can turbocharge our Arduino code with it.
   
   
   You NEED an  arduino IDE 1.5.x to be happy
   Getting Started
  See https://gcc.gnu.org/projects/cxx0x.html
  and http://permalink.gmane.org/gmane.comp.hardware.arduino.devel/3350
  You need something like
     echo 'compiler.cpp.extra_flags=-DEXTRA_CPP_FEATURE -std=c++11 -std=gnu++11  ' >$ARDUINO_HOME/hardware/arduino/avr/platform.local.txt
  to override standard arduino compiler flags and enable C++11 experimental features
  You need also to realunch your arduino ide to force reading the new options
  
  
  About C++11
   http://en.wikipedia.org/wiki/C%2B%2B11
*/

#ifndef EXTRA_CPP_FEATURE
#error "You need to create your custom $ARDUINO_HOME/hardware/arduino/avr/platform.local.txt in your ARDUINO 1.5.x IDE"
#endif

// auto keyword is like C# 'var'. A small type inference for us
const auto x=1;

// And you get for sure lambda functions...
const auto lambdaGuy = [](int i){ return i; };

void setup() {
  // 'Range-based for' syntax: no more boring for! Wellcome in 2000!
  int array[5] = { 1, 2, 3, 4, 5 };
  for (int& x : array) {
    x *= lambdaGuy(x);
  }
  
  // similar but also using type inference for array elements
  for (auto &x : array) {
      x *= 2;
  }
}

// Constant expressions....
// constexpr allows the user to guarantee that a function 
// or object constructor is a compile-time constant
constexpr int get_five() {return 5;}
int some_value[get_five() + 7]; // Create an array of 12 integers. Legal C++11

// WE LOVE NULL POINTERS
// For backwards compatibility reasons, 0 remains a valid null pointer constant.
// ^^^^ Stupid idea, anyway....
char *pc = nullptr;     // OK
// int    i = nullptr;     // error
// SOLVE:: foo(nullptr);           // calls foo(nullptr_t), not foo(int);

// Varadic templates rocks  en.wikipedia.org/wiki/Variadic_templates
// template<typename First, typename... Rest> class tuple;

// #include <functional>
// The multi write_logging guy:
namespace LogMagic {
  enum class LogLevel : int {  
    /*TRACE,*/DEBUG,INFO 
  };
  
  constexpr auto configuredLogLevel() { return LogLevel::DEBUG ; }
  
  auto write_log2String(LogLevel lv){
    switch(lv){
      case LogLevel::DEBUG:
        return "DEBUG";
      case LogLevel::INFO:
        return "INFO";
      default:
        return "?"; 
    }
  }
  
  template <typename T>
  inline auto say(const T& t) {
    Serial.println(t);
  }
    
  template <typename T, typename... Ts>
  inline auto say(const T& t, const Ts&... ts) {
      //if (sizeof...(ts) > 0) {
          Serial.println(t);
          say(ts...);
      //}
  }
  
  // Main entry point
  template <typename... Args>
  inline void write_log(LogLevel l, const Args&... a){
    if(l >= configuredLogLevel() ){
      say(write_log2String(l), a...);      
    }
  }
  
  // Handy functions
  template <typename...Stuff>
  inline void info(const Stuff&... s){
    write_log(LogLevel::INFO, s...);
  }
  
  

}// Log Magic


void loop() {
  LogMagic::info(F("Yeppa"));
  LogMagic::info(F("Zeppa"));
  
}
