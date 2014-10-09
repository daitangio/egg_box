
#include <pitches_it.h>
#include <Narcoleptic.h>

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
  analogWrite(greenLed,0);
  analogWrite(redLeds,0);
  analogWrite(blueLed,0);
  int lx= n%3;
  if(lx==0) {
    analogWrite(greenLed,255);
  }else if (lx==1){
    analogWrite(redLeds,255);
  }else {
    analogWrite(blueLed,255);
  }
  tone(speakerOut,n);
  delay(115);
  noTone(speakerOut);
  analogWrite(greenLed,0);
  analogWrite(redLeds,0);
  analogWrite(blueLed,0);

}

void playLong(int n){
  analogWrite(greenLed,0);
  analogWrite(redLeds,0);
  analogWrite(blueLed,0);
  int lx= n%3;
  if(lx==0) {
    analogWrite(greenLed,255);
  }else if (lx==1){
    analogWrite(redLeds,255);
  }else {
    analogWrite(blueLed,255);
  }
  tone(speakerOut,n);
  delay(115*(3/2));
  noTone(speakerOut);
  analogWrite(greenLed,0);
  analogWrite(redLeds,0);
  analogWrite(blueLed,0);

}


void setup(){  
 pinMode(13, OUTPUT);
 pinMode(speakerOut, OUTPUT);
 pinMode(redLeds, OUTPUT);
 pinMode(greenLed,OUTPUT);
 taDa();
 Serial.begin(9600);
 Serial.println("The 4EggBox");
 Serial.println();
}

unsigned long lastToggredLedsBlinker;




void fadeOut(int pin, int fadeAmount){
  int brightness = 255;    // how bright the LED is
//  int fadeAmount = -5;    // how many points to fade the LED by
  
  while(brightness >0 ){
    brightness = brightness + fadeAmount;
    analogWrite(pin, brightness);
    delay(DelayTime);
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
    //delay(DelayTime+(255-brightness));
    delay(DelayTime/4);
  }
}



void fadeIn(int pin){
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  
  while(brightness <255 ){
    brightness = brightness + fadeAmount;
    analogWrite(pin, brightness);
    delay(DelayTime);
  }
}


// #define DEBUG yeppa

void loop() 
{

    
    /*** FADER PART */
    
    fadeIn(redLeds);
    fadeIn(blueLed);
    fadeOut(redLeds);
    fadeIn(greenLed);
    //fadeOut(blueLed);
    fadeIn(redLeds);
    //fadeIn(blueLed);
    delay(2500);
    fadeOut3(blueLed,greenLed,redLeds);
    // turn off all
    analogWrite(greenLed,0);
    analogWrite(redLeds,0);
    analogWrite(blueLed,0);
    // Nacolectic delay...consume less power and be happy
    Narcoleptic.delay(900);
    
    //delay(DelayTime);
 
}

