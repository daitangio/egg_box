#include <Servo.h>
#include <Ping.h>

#include "pitches_it.h"

#include <NilRTOS.h>

#define DEBUG yeppa
/*
 * Derivato dalla serie di esperimenti fatti intorno al 2011,
 * SoundBot srfutta la potenza di un ArduinoMega 2560
 * Per sviluppare un piccolo robot dotato di due motori servo e di un 
 * sensore sonico di tipo Ping))) 
 */


// Robo Sound 5 Si ferma se davanti c'è un ostacolo troppo vicino
// non fa altro
// Ha la correzione angolare
// E' più veloce 
// **SEMBRA**  che la libreria Aiko dia problemi al controllo motori



const int pingServoEnginePin=4; 
const int leftEnginePin=11;
const int rightEnginePin=6;
const int pingPin=5; // Arancione
const int distanceFeedbackLed = 13;  // PWM
const int speakerOut=A5;
const boolean DEBUG_MODE=true;


// Costanti da tarare

const int CrashDistance=15 ; // 15cm è la distanza della testa del ping dal corpo. Se siamo a 15 siamo andati contro l'ostacolo
const int DangerDistance=25 ; //
const int ServoWaitMS=200;
const int Front_Angle=90-12;  // 10..15 Rappresenta i 90 Gradi

// To fix, these are still beta:
const int Right_Angle=0;
const int Left_Angle=180-12;

Servo crServo;
Servo leftEngineServo;
Servo rightEngineServo;
Ping ping = Ping(pingPin,0,0);



void setup(){
 Serial.begin(9600);
 crServo.attach(pingServoEnginePin);
 leftEngineServo.attach(leftEnginePin);
 rightEngineServo.attach(rightEnginePin);
 
 pinMode(distanceFeedbackLed, OUTPUT);
 pinMode(speakerOut, OUTPUT);
 taDa();
 Serial.print("C:");
 Serial.println(DangerDistance);
 nilSysBegin();
}

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





boolean look(int angle){
   crServo.write(angle);
  nilThdSleepMilliseconds( ServoWaitMS );
  return isDangerDirection();
}

// NILRTOS Threads.
// Master thread for Ping, other for engines (disabled right now)

NIL_WORKING_AREA(waPingHead, 128);
NIL_THREAD(PingHead, arg) {
  
  while(true) {
    
    leftEngineServo.write(90); // STOP
    rightEngineServo.write(90);
    
    // Scan for direction:
    // Left, Front or Right?
    look(Front_Angle);
    if(isDangerDirection()){
      Serial.println(F("Front is bad"));
      look(Right_Angle);
      
      if(isDangerDirection()){
        Serial.println(F("Right is bad"));
        look(Left_Angle);
        if(isDangerDirection()){
          Serial.print(F("Left is BAD: Blocked, try back"));
          // TODO Back action
        }else{
          // TODO Go left
        } 
      }else {
        // Go Right: keep right engine fix and move left at full power.
        // TODO 
      }
    }else {
      /// Go front
      leftEngineServo.write(110);
      rightEngineServo.write(180-110);
    }
    
    // Sleep a bit?
    nilThdSleepMilliseconds( ServoWaitMS*2 );
  } // while main loop
}

/*
NIL_WORKING_AREA(waEngine, 128);
NIL_THREAD(Engine, arg) {
  // Basic code to stay quite while the head work out the best direction
  while(true) {
   leftEngineServo.write(90); // STOP
   rightEngineServo.write(90);
   nilThdSleepMilliseconds(1000);
  }
}
*/
  
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL            , PingHead, NULL, waPingHead, sizeof(waPingHead))
//NIL_THREADS_TABLE_ENTRY(NULL            , Engine, NULL, waEngine, sizeof(waEngine))
NIL_THREADS_TABLE_END()


// Boot Music
void taDa(){
  crServo.write(Front_Angle-45);
  tone(speakerOut,NOTE_SI4);
  delay(100);
  tone(speakerOut,NOTE_LA3);
  delay(100);
  tone(speakerOut,NOTE_DO2);
  delay(100);
  noTone(speakerOut);
  crServo.write(Front_Angle+45);

}


void old_loop()
{
  int temp;
  
  
  // GOOO
  leftEngineServo.write(110);
  rightEngineServo.write(180-110);
  crServo.write(Front_Angle);
  delay(ServoWaitMS);
  if(isDangerDirection()){
    leftEngineServo.write(90); // STOP
    rightEngineServo.write(90);
    while(isDangerDirection()){    
      noTone(speakerOut);      
    }
  } 
  /*
  crServo.write(45);
  delay(ServoWaitMS);
  isDangerDirection(); 

  crServo.write(90);
  delay(ServoWaitMS);
  isDangerDirection(); 

  crServo.write(90+45);
  delay(ServoWaitMS);
  isDangerDirection(); 
  */
  
  
}


boolean isDangerDirection(){
  double cm=fire();
  
  if( cm <= DangerDistance ){
    Serial.print("D"); 
    return true;
  }else{
    return false;
  }
}


void ledFeedback(double cm){
  digitalWrite(distanceFeedbackLed, HIGH); 
  delay(cm * 5 ); // each centimeter adds some milliseconds delay 
  digitalWrite(distanceFeedbackLed, LOW); 
  delay( cm * 5);
}

void toneFeedback(double cm){
  int thisPitch = map(cm, 5, 300, /*<-Risoluzine ping vs risoluzione speaker-> */
              NOTE_LA3/*NOTE_B0*/, NOTE_LA7);
  //Serial.print(" Pitch =");
  //Serial.print(thisPitch);    
  tone(speakerOut,thisPitch /*NOTE_LA5*/);
  if(cm >100){
    noTone(speakerOut);
  }
}

double fire(){
      delay(ServoWaitMS);
      ping.fire();
      double cm=ping.centimeters();
      ledFeedback(cm);
      //toneFeedback(cm);
      if(DEBUG_MODE){
        Serial.print("H ");
        Serial.print(crServo.read());
        Serial.print(")");
       
        Serial.println(cm);
      
        if(cm<1){
            Serial.print(" E ");                 
        }
        Serial.println("");
      }
      return cm;
}

