/*
  Blink evolution is here!
  The system will sleep until a special button is pressed


 */

#include <NilRTOS.h>

// #define DEBUG yeppa
//#define DEBUG_LOG yeppa
#include <jj-log.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int aliveLed = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(aliveLed, OUTPUT);   
  Serial.begin(9600);
  // l("2021 Led Dreams");
  nilSysBegin();
}


void loop() {
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

const int DelayTime=44; // 44 is good

NIL_WORKING_AREA(waBreathLed, 8);
NIL_THREAD(BreathLed,arg){
  const int minBright=25;
  const int maxBright=255;
  while(true){
    int pin=aliveLed;
    int brightness = minBright;    // how bright the LED is
    int fadeAmount = 5;    // how many points to fade the LED by
  
    analogWrite(pin, brightness);
    // fade in,,,,
    while(brightness <maxBright ){
      brightness = brightness + fadeAmount;
      analogWrite(pin, brightness);
      nilThdSleepMilliseconds(DelayTime);
    }
    
    // fad out...
    while(brightness > minBright ){
      brightness = brightness - fadeAmount;
      analogWrite(pin, brightness);
      nilThdSleepMilliseconds(DelayTime);
    }
    
    
  }    
}

NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL            , BreathLed, NULL, waBreathLed, sizeof(waBreathLed))
NIL_THREADS_TABLE_END()

// Local variables:
// mode:c++
// mode:company
// End:
