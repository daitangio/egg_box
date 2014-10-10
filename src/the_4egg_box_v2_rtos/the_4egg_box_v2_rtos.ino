

#include <pitches_it.h>
#include <NilRTOS.h>


const int speakerOut=A5;

/*** FADER**/
int redLeds = 9;           // the pin that the LED is attached to
int greenLed=6;      // Reverse pin
int blueLed=5;  //ledgroup 3 
const int DelayTime=35; // 44 is good




// Boot Music
void taDa(){
  
  //Serial.println("TADA!");
  play(NOTE_SI5);
  play(NOTE_LA4);
  play(NOTE_DO3);
  
  playLong(NOTE_RE3);
  play(NOTE_MI3);
  play(NOTE_FA3);
  play(NOTE_RE3);
  //play(NOTE_MI2);
  /*
  play(NOTE_FA2);
  play(NOTE_SOL2);
  play(NOTE_LA2);
  play(NOTE_SI2);*/
  playLong(NOTE_DO2);
  
}

//TODO:  Rotating flashes for super duper start
//Make a ball of light spinning under the 3 led, slower then faster then disapear


void play(int n){
  tone(speakerOut,n);
  nilThdSleepMilliseconds(115);
  noTone(speakerOut);

}

void playLong(int n){
  tone(speakerOut,n);
  nilThdSleepMilliseconds(115*(3/2));
  noTone(speakerOut);

}


void setup(){  
 pinMode(13, OUTPUT);
 pinMode(speakerOut, OUTPUT);
 pinMode(redLeds, OUTPUT);
 pinMode(greenLed,OUTPUT);

 Serial.begin(9600);
 Serial.println("The 4EggBox v2 RTOS");
 Serial.println();
 nilSysBegin();
}

unsigned long lastToggredLedsBlinker;




void fadeOut(int pin, int fadeAmount){
  int brightness = 255;    // how bright the LED is
//  int fadeAmount = -5;    // how many points to fade the LED by
  
  while(brightness >0 ){
    brightness = brightness + fadeAmount;
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


#define DEBUG yeppa


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


// Declare a stack with 128 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waMusic, 128);

NIL_THREAD(Music, arg) {
  while(true){
    taDa();
    // Sleep for 10 sec
    nilThdSleepMilliseconds(10000);  
  }
}


NIL_WORKING_AREA(waBlinkingLights, 128);
NIL_THREAD(BlinkingLights,arg){
  while(true){
    
    /*** FADER PART */
    
    fadeIn(redLeds);
    fadeIn(blueLed);
    fadeOut(redLeds);
    fadeIn(greenLed);
    //fadeOut(blueLed);
    fadeIn(redLeds);
    //fadeIn(blueLed);
    nilThdSleepMilliseconds(2500);
    fadeOut3(blueLed,greenLed,redLeds);
    // turn off all
    analogWrite(greenLed,0);
    analogWrite(redLeds,0);
    analogWrite(blueLed,0);
    // Give some time
    nilThdSleepMilliseconds(900);
    
    //nilThdSleepMilliseconds(DelayTime);
  }
}

/** Thread static table 
  A thread's priority is
  determined by its position in the table with highest priority first.
*/
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL /*TH NAME*/, Music,          NULL, waMusic,          sizeof(waMusic))
NIL_THREADS_TABLE_ENTRY(NULL            , BlinkingLights, NULL, waBlinkingLights, sizeof(waBlinkingLights))
NIL_THREADS_TABLE_END()


