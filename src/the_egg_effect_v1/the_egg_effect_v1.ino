// HC RS04 Sensore ultrasuoni
const int triggerPort = 7;
const int echoPort = 8;
const int blueLed=11;
const int redLed=10;
const int maxDistCm=28; // was 28
#include <NilRTOS.h>
#include <jj-log.h>

void setup() {
   
  pinMode( triggerPort, OUTPUT );
  pinMode( echoPort, INPUT );
  pinMode( blueLed, OUTPUT);
  pinMode( redLed, OUTPUT);
  Serial.begin( 9600 );
  Serial.println( "Sensore ultrasuoni: ");
  nilSysBegin();
}
 
void loop() {
  //Serial.println();
  //nilPrintUnusedStack(&Serial);
  
}

NIL_WORKING_AREA(waUltrasonicSensor, 74);
NIL_THREAD(UltrasonicSensor,arg){
  static long lastValue=0;
  while(true) {
    //porta bassa l'uscita del trigger
    digitalWrite( triggerPort, LOW );
   
    //invia un impulso di 10microsec su trigger
    digitalWrite( triggerPort, HIGH );
    delayMicroseconds( 10 ); // nilThdSleepMicorseconds
    digitalWrite( triggerPort, LOW );
   
    long duration = pulseIn( echoPort, HIGH );
   
    long r = 0.034 * duration / 2;
    if(r != lastValue){
    
      Serial.print( F("durata: ") );
      Serial.print( duration );
      Serial.print( F(" , ") );
      Serial.print( F("distanza: ") );
     
      //dopo 38ms è fuori dalla portata del sensore
      if( duration > 38000 ) Serial.println( "fuori portata");
      else { 
      
        Serial.print( r ); Serial.print( F("cm") );
      
        Serial.print(" ");
        nilPrintUnusedStack(&Serial);
        if(r <=maxDistCm && r>=3 ){
          int v=map(r,3,maxDistCm,0,255);
          Serial.print( v );
          Serial.print(F(" LED "));
          analogWrite(blueLed, v);
          
        }
      
      }
      lastValue=r;
    }
  
    /* From sensor data sheet:
       we suggest to use over 60ms
       measurement cycle, in order to prevent trigger signal 
       to the echo signal

       60*5 --> too slow
       60*2 --> perfect
    */
  
    nilThdSleepMilliseconds( 60*2 );
  
  }
}

/** NB: Fabio's Led need not a resistence 
 *  but for better security we reduce power down to 50% of the duty cycle.
 */
const byte Rmax=128;
NIL_WORKING_AREA(waRedCounter, 74);
NIL_THREAD(RedCounter,arg){
  static byte redValue=Rmax;
  static byte verso=-1;
  while(true){
    analogWrite(redLed,redValue);
    redValue+= verso*1;
    nilThdSleepMilliseconds(10);
    if(redValue ==0 || redValue ==Rmax) {
      verso *=-1;
    }
  }
}


NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL /*TH NAME*/, UltrasonicSensor,          NULL, waUltrasonicSensor,    sizeof(waUltrasonicSensor))
NIL_THREADS_TABLE_ENTRY(NULL /*TH NAME*/, RedCounter,                NULL, waRedCounter,          sizeof(waRedCounter))
NIL_THREADS_TABLE_END()

/*
 * Local variables:
 * mode: c++
 * mode: flymake
 * End:
 */
