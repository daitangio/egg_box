#include <Servo.h>
#include <Ping.h>

#include "pitches_it.h"


// Robo Sound 5 Si ferma se davanti c'è un ostacolo troppo vicino
// non fa altro
// Ha la correzione angolare
// E' più veloce 
// **SEMBRA**  che la libreria Aiko dia problemi al controllo motori



const int pingServoEnginePin=4; 
const int leftEnginePin=11;
const int rightEnginePin=6;
const int pingPin=5; //Ma forse si può fare anche 7!
const int distanceFeedbackLed = 13; 
const int speakerOut=A5;
const boolean DEBUG_MODE=true;


// Costanti da tarare

const int CrashDistance=15 ; // 15cm è la distanza della testa del ping dal corpo. Se siamo a 15 siamo andati contro l'ostacolo
const int DangerDistance=25 ; //
const int ServoWaitMS=200;
const int Front_Angle=90-12;  // 10..15 Rappresenta i 90 Gradi

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
}


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


void loop()
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
      //ledFeedback(cm);
      toneFeedback(cm);
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

