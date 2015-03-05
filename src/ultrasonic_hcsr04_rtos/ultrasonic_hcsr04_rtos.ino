//HC RS04 Sensore ultrasuoni
int triggerPort = 7;
int echoPort = 8;

#include <NilRTOS.h>
#include <jj-log.h>

void setup() {
   
  pinMode( triggerPort, OUTPUT );
  pinMode( echoPort, INPUT );
  Serial.begin( 9600 );
  Serial.println( "Sensore ultrasuoni: ");
  nilSysBegin();
}
 
void loop() {
  //nilPrintUnusedStack(&Serial);
  //Serial.println();
}

NIL_WORKING_AREA(waUltrasonicSensor, 64);
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
    
    Serial.print( "durata: " );
    Serial.print( duration );
    Serial.print( " , " );
    Serial.print( "distanza: " );
     
    //dopo 38ms è fuori dalla portata del sensore
    if( duration > 38000 ) Serial.println( "fuori portata");
    else { 
      
        Serial.print( r ); Serial.print( "cm" );
      
        Serial.print(" ");
        nilPrintUnusedStack(&Serial);
      
      
      }
    lastValue=r;
  }
  //aspetta mezzo secondo ca
  /* we
      suggest to use over 60ms
      measurement cycle, in order to prevent trigger sign
      al to the echo signal
*/
  nilThdSleepMilliseconds( 60*2 );
  }
}

NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL /*TH NAME*/, UltrasonicSensor,          NULL, waUltrasonicSensor,          sizeof(waUltrasonicSensor))
NIL_THREADS_TABLE_END()
