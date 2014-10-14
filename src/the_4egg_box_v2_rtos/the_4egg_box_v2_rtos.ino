
/** EGG BOX WITH 4 Independent PWM light and one powerful speaker.
  * Red Led rocks a lot
  */
#include <pitches_it.h>
#include <NilRTOS.h>

#define DEBUG yeppa

const int speakerOut=A5;

/*** FADER**/
int yellowLed = 9;           // the pin that the LED is attached to
int greenLed=6;      // Reverse pin
int blueLed=5;  //ledgroup 3 
int redLed=10;  //PWM 3 get conflict with sound
const int DelayTime=44; // 44 is good

const float  durationBase=750;
const float D1_3= 1/3.0;
const float HALF= 0.5;
const float Q   = 0.25;

// Two pair: note and duration
const float music[] /*PROGMEM*/ ={ NOTE_FA4,1, 
                      NOTE_LAS4 /*BEMOLLE*/ ,1, 
                      
                      NOTE_FA4,  D1_3,
                      NOTE_FA4,  D1_3,
                      NOTE_LAS4, D1_3, /*SI BEM*/
                      
                      NOTE_FA4,  1/2.0,
                      NOTE_RES4, 1/2.0, /* MI BEM*/
                      
                      // Second 
                      NOTE_FA4,1,
                      NOTE_DO5,1,
                      
                      NOTE_FA4,D1_3,
                      NOTE_FA4,D1_3,
                      NOTE_DOS5,D1_3,
                      
                      NOTE_DOS5,HALF, // INEXACT....
                      NOTE_LAS4,HALF, // LAB
                       
                      
                      
                      //THIRD
                      NOTE_FA4,Q,
                      NOTE_DO5,Q,
                      NOTE_FA5,Q,
                      NOTE_FA4,Q,
                      
                      NOTE_RES4,D1_3,     //MI bem                     
                      NOTE_MI4,D1_3,
                      NOTE_DO4,D1_3,
                      
                      
                      NOTE_SOL4,HALF,
                      NOTE_FA4,HALF,
                      NOTE_FA4,2,
                      //+ PAUSA                      
                      -1,-1,-1,-1
                      
                     
                  };

const float noteSwifter=1 ; // Default 1
// Boot Music
void taDa(){
 
  #ifdef DEBUG
    Serial.print("MUSIC STARTS. Total Data:");
  #endif
  
  for (int i=0; music[i] != -1;) {
    noTone(speakerOut);
    int note=(int)( ((float)music[i])  *noteSwifter);
    int duration=((int) (music[i+1]*durationBase))+1;
    #ifdef DEBUG
    Serial.print(i); Serial.print(" - Note:");
    Serial.print(note); Serial.print(" Dur:");
    Serial.println(duration);    
    #endif
    tone(speakerOut,note);
    nilThdSleepMilliseconds(duration);
    noTone(speakerOut);
    i+=2;
    
  }
  noTone(speakerOut);
  #ifdef DEBUG
  Serial.println("MUSIC ENDS");
  #endif
 
  
}



void setup(){  
 pinMode(13, OUTPUT);
 pinMode(speakerOut, OUTPUT);
 pinMode(yellowLed, OUTPUT);
 pinMode(greenLed,OUTPUT);
 pinMode(redLed,OUTPUT);

 Serial.begin(9600);
 Serial.println("The 4EggBox v2 RTOS");
 Serial.println();
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


// Declare a stack with 128 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waMusic, 128);

NIL_THREAD(Music, arg) {
  taDa();  
  while(true){
    
    // Sleep for 10 sec
    nilThdSleepMilliseconds(60000);  
  }
}


NIL_WORKING_AREA(waBlinkingLights, 128);
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

// Very tiny stack for this red alerter
NIL_WORKING_AREA(waBlinkingRed, 64);
NIL_THREAD(BlinkingRed,arg){
  const int minBright=100;
  const int maxBright=255;
  while(true){
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
    
    // fad out...
    while(brightness > minBright ){
      brightness = brightness - fadeAmount;
      analogWrite(pin, brightness);
      nilThdSleepMilliseconds(DelayTime);
    }
    
    
  }
}


/** Thread static table 
  A thread's priority is
  determined by its position in the table with highest priority first.
*/
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL /*TH NAME*/, Music,          NULL, waMusic,          sizeof(waMusic))
NIL_THREADS_TABLE_ENTRY(NULL            , BlinkingLights, NULL, waBlinkingLights, sizeof(waBlinkingLights))
NIL_THREADS_TABLE_ENTRY(NULL            , BlinkingRed, NULL, waBlinkingRed, sizeof(waBlinkingRed))
NIL_THREADS_TABLE_END()


