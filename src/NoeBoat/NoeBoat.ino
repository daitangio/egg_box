
/**  The ultra tight Noe Boat
  *  You NEED an  arduino IDE 1.5.x to be happy
  * and a special file to enable C++ extra features:
  * echo 'compiler.cpp.extra_flags=-DEXTRA_CPP_FEATURE -std=c++11 -std=gnu++11  ' >$ARDUINO_HOME/hardware/arduino/avr/platform.local.txt
  * Board: arduino uno
  * This is the firt example using lambda function and heavy auto keyword to use them. Type safety and no anoyny type with you.
  * 
  */

#ifndef EXTRA_CPP_FEATURE
#error "You need to create your custom $ARDUINO_HOME/hardware/arduino/avr/platform.local.txt in your ARDUINO 1.5.x IDE"
#endif


#include <pitches_it.h>
#include <NilRTOS.h>

// #define DEBUG yeppa

// const int speakerOut=A5;

/*** FADER**/
int yellowLed = 9;           // the pin that the LED is attached to
int greenLed=6;      // Reverse pin
int blueLed=5;  //ledgroup 3 
int redLed=10;  // PWM 3 get conflict with sound
const int DelayTime=44; // 44 is good

/// See http://en.cppreference.com/w/cpp/language/lambda
/// For complete syntax explanation
#define DEBUG
#ifdef DEBUG 
auto say=[](String p1){ Serial.print(p1); };
#else
auto say=[](String p1)->void{  };
#endif



/// FUNCTIONAL MUBO JUMBO please see ./bits/stl_function.h

template<typename _Arg, typename _Result>
    struct unary_function
    {
      /// @c argument_type is the type of the argument
      typedef _Arg 	argument_type;   

      /// @c result_type is the return type
      typedef _Result 	result_type;  
    };

template<typename _Arg1, typename _Arg2, typename _Result>
    struct binary_function
    {
      /// @c first_argument_type is the type of the first argument
      typedef _Arg1 	first_argument_type; 

      /// @c second_argument_type is the type of the second argument
      typedef _Arg2 	second_argument_type;

      /// @c result_type is the return type
      typedef _Result 	result_type;
    };
    
template<typename _Tp>
    struct plus : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      inline operator()(const _Tp& __x, const _Tp& __y) const
      { return __x + __y; }
    };



  template<typename _Predicate>
    class unary_negate
    : public unary_function<typename _Predicate::argument_type, bool>
    {
    protected:
      _Predicate _M_pred;

    public:
      explicit
      unary_negate(const _Predicate& __x) : _M_pred(__x) { }

      bool
      operator()(const typename _Predicate::argument_type& __x) const
      { return !_M_pred(__x); }
    };






void setup(){  
 pinMode(13, OUTPUT);
 //pinMode(speakerOut, OUTPUT);
 pinMode(yellowLed, OUTPUT);
 pinMode(greenLed,OUTPUT);
 pinMode(redLed,OUTPUT);

 Serial.begin(9600);
 Serial.println(F("LedCheck"));
 Serial.println();
 
 int pin2test[]= { yellowLed, greenLed, redLed };
 digitalWrite(13, HIGH); 
 for(int& pin: pin2test){
   analogWrite(pin,255);
   say(".");
   delay(500);
 }
 /*
 plus<int> bho;
 digitalWrite(bho(11,2), LOW); 
 */
 
 
 //[capture](parameters)->return-type{body}
 auto inc= [](int x) -> int {return x+1;};
 digitalWrite(inc(12),LOW);
 say("#");
 Serial.println(F("The NoeBox v0.1+ RTOS"));
 
 
 
 nilSysBegin();
}

unsigned long lastToggyellowLedBlinker;




void fadeOut(int pin, int fadeAmount){
 
  int brightness = 255;    // how bright the LED is  
  while(brightness >0 ){
    brightness = brightness + fadeAmount;
    if(brightness<0) { brightness=0;};
    analogWrite(pin, brightness);
    nilThdSleepMilliseconds(DelayTime);
  }
}

void fadeOut(int pin){
  fadeOut(pin, -5);
}

// Very slow, very nice
void fadeOut3(int pin1,int pin2, int pin3){
  int brightness = 255;    // how bright the LED is
  int fadeAmount = -2;    // how many points to fade the LED by
  
  while(brightness >2 ){
    brightness = brightness + fadeAmount;
    analogWrite(pin1, brightness);
    analogWrite(pin2, brightness);
    analogWrite(pin3, brightness);
    //nilThdSleepMilliseconds(DelayTime+(255-brightness));
    nilThdSleepMilliseconds(DelayTime/4);
  }
}



void fadeIn(int pin){
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  
  while(brightness <255 ){
    brightness = brightness + fadeAmount;
    analogWrite(pin, brightness);
    nilThdSleepMilliseconds(DelayTime);
  }
}





#ifdef DEBUG
  // Use tiny unbuffered NilRTOS NilSerial library.
  #include <NilSerial.h>
  // Macro to redefine Serial as NilSerial to save RAM.
  // Remove definition to use standard Arduino Serial.
  #define Serial NilSerial

#endif
void loop() 
{
    // Used only on DEBUG:
    #ifdef DEBUG
      nilPrintStackSizes(&Serial);
      nilPrintUnusedStack(&Serial);
      Serial.println();
    
      // Delay for one second.
      // Must not sleep in loop so use nilThdDelayMilliseconds().
      // Arduino delay() can also be used in loop().
      nilThdDelayMilliseconds(1000);
    #endif
}





NIL_WORKING_AREA(waBlinkingLights, 64);
NIL_THREAD(BlinkingLights,arg){
  //fadeIn(redLed);
  while(true){
    
    /*** FADER PART */
    
    fadeIn(yellowLed);
    fadeIn(blueLed);
    fadeOut(yellowLed);
    fadeIn(greenLed);
    
    //fadeOut(blueLed);
    fadeIn(yellowLed);
    //fadeIn(blueLed);
    nilThdSleepMilliseconds(2500);
    fadeOut3(blueLed,greenLed,yellowLed);
    // turn off all
    analogWrite(greenLed,0);
    analogWrite(yellowLed,0);
    analogWrite(blueLed,0);
    // Give some time
    nilThdSleepMilliseconds(900);
    
    //nilThdSleepMilliseconds(DelayTime);
  }
}

//////
inline auto makeFadeInFunctionOnPin(int pin, int fadeAmount){
  return [=]() {
    int brightness = 0;    // how bright the LED is
    while(brightness <255 ){
      brightness = brightness + fadeAmount;
      analogWrite(pin, brightness);
      nilThdSleepMilliseconds(DelayTime);
    }
  };
}

inline auto makeFadeOutFunctionOnPin(int pin, int fadeAmount){
  return [=](){
   
    int brightness = 255;    // how bright the LED is  
    while(brightness >0 ){
      brightness = brightness - fadeAmount;
      if(brightness<0) { brightness=0;};
      analogWrite(pin, brightness);
      nilThdSleepMilliseconds(DelayTime);
    }
  };
}


// Very tiny stack for this red alerter: we economize on the rest
NIL_WORKING_AREA(waBlinkingRed, 8);
NIL_THREAD(BlinkingRed,arg){
  const int minBright=25;
  const int maxBright=255;
  auto fadeInF=makeFadeInFunctionOnPin(redLed,5);
  auto fadeOutF=makeFadeOutFunctionOnPin(redLed, 7);
  while(true){
    fadeInF();
    fadeOutF();
    
    /*
    digitalWrite(13, HIGH); 
    int pin=redLed;
    int brightness = minBright;    // how bright the LED is
    int fadeAmount = 5;    // how many points to fade the LED by
  
    analogWrite(pin, brightness);
    // fade in,,,,
    while(brightness <maxBright ){
      brightness = brightness + fadeAmount;
      analogWrite(pin, brightness);
      nilThdSleepMilliseconds(DelayTime);
    }
    digitalWrite(13, LOW); 
    // fad out...
    while(brightness > minBright ){
      brightness = brightness - fadeAmount;
      analogWrite(pin, brightness);
      nilThdSleepMilliseconds(DelayTime);
    }
    */
    
    
  }
}


/** Thread static table 
  A thread's priority is
  determined by its position in the table with highest priority first.
*/
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL            , BlinkingLights, NULL, waBlinkingLights, sizeof(waBlinkingLights))
NIL_THREADS_TABLE_ENTRY(NULL            , BlinkingRed, NULL, waBlinkingRed, sizeof(waBlinkingRed))
NIL_THREADS_TABLE_END()


