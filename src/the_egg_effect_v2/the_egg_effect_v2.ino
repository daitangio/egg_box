// HC RS04 Sensore ultrasuoni
const int triggerPort = 7;
const int echoPort = 8;
const int blueLed=11;
//const int blueLed2=11;
const int redLed=2;
const int maxDistCm=28; 
const int minDistCm=10;

#include <NilRTOS.h>
#include <NilSerial.h>
#include <jj-log.h>
/**
 * TODO USARE DUE LED PER MIGLIORE LUCE DA LONTANO
 * PENSARE AD ALTRI EFFETTI DI LUCE GRADEVOLI
 */

void setup() {
   
  pinMode( triggerPort, OUTPUT );
  pinMode( echoPort, INPUT );
  pinMode( blueLed, OUTPUT);
  Serial.begin( 9600 );
  //Serial.println( F( "Sensore ultrasuoni: "));
  nilSysBegin();
}
 
void loop() {
  //Serial.println();
  //nilPrintUnusedStack(&Serial);
  
}

NIL_WORKING_AREA(waUltrasonicSensor, 84); // 74 was ok but enlarged for better control
NIL_THREAD(UltrasonicSensor,arg){
  static long lastValue=0;
  analogWrite(blueLed, 255);
  nilThdSleepMilliseconds(2000);
  while(true) {
    //porta bassa l'uscita del trigger
    digitalWrite( triggerPort, LOW );
   
    //invia un impulso di 10microsec su trigger
    digitalWrite( triggerPort, HIGH );
    delayMicroseconds( 10 ); // nilThdSleepMicorseconds
    
    
    digitalWrite( triggerPort, LOW );
   
    long duration = pulseIn( echoPort, HIGH );
   
    float r = 0.034 * duration / 2;
    if(r != lastValue){
    
      Serial.print( F("dur: ") );
      Serial.print( duration );
      Serial.print( F("> ") );
     
      //dopo 38ms è fuori dalla portata del sensore
      if( duration > 38000 ) Serial.println( "fuori portata");
      else { 
      
        Serial.print( r ); Serial.print( F("cm") );
        long rlong=(long)r*10;      
        Serial.print(F(" >Res"));
        Serial.print(rlong);
        int v=map(rlong,minDistCm*10,maxDistCm*10,0,255);
        Serial.print(F(" "));
        Serial.print( v );
        if(r <=maxDistCm && r>=minDistCm ){
          Serial.print(F(" *"));
          analogWrite(blueLed, v);
        }else{
          // TODO: Push the old one (lastValue)
          // Serial.print(F(".."));
        }
        nilPrintUnusedStack(&Serial);
      }
      lastValue=r;
    }
  
    /* From sensor data sheet:
       we suggest to use over 60ms
       measurement cycle, in order to prevent trigger signal 
       to the echo signal

       60*5 --> too slow
       60*2 --> good until 28 cm
       NB: Interrupt could lead to fake measure, so we 
    */

    nilThdSleepMilliseconds( 60*2 );
    
  }
}


/** NB: Fabio's Led need not a resistence 
 *  but for better security we reduce power down to 50% of the duty cycle.
 */

NIL_WORKING_AREA(waRedCounter, 69);
NIL_THREAD(RedCounter,arg){
  pinMode(redLed,OUTPUT);
  while(true){
    digitalWrite(redLed,HIGH);
    nilThdSleepMilliseconds(250 /*/2*/);    
    digitalWrite(redLed,LOW);
    nilThdSleepMilliseconds(250);
  }
}

/*
const byte Rmax=255;
NIL_WORKING_AREA(waBlueMagic, 74);
NIL_THREAD(BlueMagic,arg){
  static byte redValue=Rmax;
  static byte verso=-1;
  while(true){
    analogWrite(blueLed,redValue);
    redValue+= verso*1;
    nilThdSleepMilliseconds(10);
    if(redValue ==0 || redValue ==Rmax) {
      verso *=-1;
    }
  }
}
*/


NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL /*TH NAME*/, UltrasonicSensor,          NULL, waUltrasonicSensor,    sizeof(waUltrasonicSensor))
//NIL_THREADS_TABLE_ENTRY(NULL, BlueMagic,                NULL, waBlueMagic,          sizeof(waRedCounter))
NIL_THREADS_TABLE_ENTRY(NULL, RedCounter,                NULL, waRedCounter,          sizeof(waRedCounter))
NIL_THREADS_TABLE_END()

/*
 * Local variables:
 * mode: c++
 * mode: flymake
 * End:
 */
