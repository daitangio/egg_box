
/**  The ultra tight Noe Boat
  *  You NEED an  arduino IDE 1.5.x to be happy
  * and use the Special Makefile included.
  * For using with a bare naked arduino ide, try this
  * echo 'compiler.cpp.extra_flags=-DEXTRA_CPP_FEATURE -std=c++11 -std=gnu++11  ' >$ARDUINO_HOME/hardware/arduino/avr/platform.local.txt
  * Board: arduino uno
  * This is the first example using lambda function and heavy auto keyword to use them. 
  * 
  * 
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
const int DelayTime=44*3; // 44 is good





/// See http://en.cppreference.com/w/cpp/language/lambda
/// For complete syntax explanation
/// Please see also 
/// http://en.cppreference.com/w/cpp/language/function#Return_type_deduction
/// for type declaration inference and so on
// #define DEBUG
#ifdef DEBUG 
auto say=[](String p1){ Serial.print(p1); };
#else
auto say=[](String p1)->void{  };
#endif



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

 
 digitalWrite(13,LOW);
 say("#");
 Serial.println(F("The NoeBox v0.2+ RTOS"));
 
 
 
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


/** Create a fade in function based on pin and fadeAmount specified
 * @return a function you can invoke to execute a full fade in
 */
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
/* @todo  Change the fade lighting game using a sensor!
 */
NIL_THREAD(BlinkingRed,arg){
  
  auto fadeInF=makeFadeInFunctionOnPin(redLed,5);
  auto fadeOutF=makeFadeOutFunctionOnPin(redLed, 7);
  while(true){
    fadeInF();
    fadeOutF();
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

/*
 * Local variables:
 * mode: c++
 * mode: flymake
 * End:
 */
