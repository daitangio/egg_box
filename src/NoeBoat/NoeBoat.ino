
/**  The ultra tight Noe Boat
  *  You NEED an  arduino IDE 1.5.x to be happy
  * and use the Special Makefile included.
  * For using with a bare naked arduino ide, try this
  * echo 'compiler.cpp.extra_flags=-DEXTRA_CPP_FEATURE -std=c++11 -std=gnu++11  ' >$ARDUINO_HOME/hardware/arduino/avr/platform.local.txt
  * Board: arduino uno
  * 
  * Using a Led as 'poor man light sensor' the lamp after a small calibration will try to be brigther on dark and turn off on light.
  * A bunch of extra effect are on the way.
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
 
 for(int& pin: pin2test){
   analogWrite(pin,255);
   say(".");
   delay(100);
 }

 
 
 say("#");
 Serial.println(F("The NoeBox v0.3+ RTOS"));
 
 
 
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


NIL_WORKING_AREA(waLedSensor, 232);
/* Very nice idea here http://www.instructables.com/id/LED-as-lightsensor-on-the-arduino/
 * Also better implemented here
 *  http://www.instructables.com/id/Arduino-Use-LED-as-a-light-sensor/
 */
const int sensorOutput=13;
NIL_THREAD(LedSensor,arg){

  pinMode(sensorOutput, OUTPUT);
  
  int basis = 0;
  int sens = 0;
  int x = 0;
  int y = 0;
  int totaal = 0;
  int totaal1 = 0;

  // We do 50 reads to calibreate the sensor (?)
  for(x =0; x < 50; x++) {
    sens = analogRead(0);
    totaal = totaal1 + sens; totaal1 = totaal;
  }
  /// Then give a small chance to guys to work with us
  nilThdSleepMilliseconds(10);
  say(F("Calibrated"));
  while(true){
    sens = totaal/x;                          // divide the 50 readings by 50 again 
    totaal = 0;
    totaal1 = 0;
    basis = sens-20;           // setting sensitivity - now it will react if the LED is 20 lower than the setting above
    Serial.print("Base:");
    Serial.println(basis);
    
    for(y=0;y<1000;y++){       // after every 1000 tests the program will reset the led to cope with changing light
      for(x =0; x < 50; x++) {      // 50 readings to see if the sensor is in the dark
        sens = analogRead(0);
        totaal = totaal1 + sens;
        totaal1 = totaal;
        delay(10);
      }
      sens = totaal/x;
      if (sens < basis)                // testing is the led was in the dark
        digitalWrite(sensorOutput, HIGH);  // turning the led in port 13 or on the board on if the sensor-led was 20 darker than now than in the setting
      else  
        digitalWrite(sensorOutput, LOW); // turning it of if not

      // Now use the value on redLed
      Serial.print("$");
      Serial.print(sens);
      // Se basis=182, sens varia tra 187 e 174
      // Per cui dobbiamo mappare il dim tra una variazione di 187-174 = 13 valori da riproiettare su 255
      // Purtroppo al momento no è parametrico, valutare come centrarlo
      int remapped=255-map(sens,167,184,0,255);
      if(remapped>255) remapped=255;
      else if (remapped <0) remapped=0;
      
      Serial.print(" R:");
      Serial.println(remapped);
      analogWrite( redLed, remapped);
      totaal = 0;  
      totaal1 = 0;  
      nilThdSleepMilliseconds(100);
      }  //for reset
  } // while(true)
}




/** Thread static table 
  A thread's priority is
  determined by its position in the table with highest priority first.
  
*/
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL            , LedSensor, NULL, waLedSensor, sizeof(waLedSensor))
//NIL_THREADS_TABLE_ENTRY(NULL            , BlinkingLights, NULL, waBlinkingLights, sizeof(waBlinkingLights))
//NIL_THREADS_TABLE_ENTRY(NULL            , BlinkingRed, NULL, waBlinkingRed, sizeof(waBlinkingRed))
NIL_THREADS_TABLE_END()

/*
 * Local variables:
 * mode: c++
 * mode: flymake
 * End:
 */
