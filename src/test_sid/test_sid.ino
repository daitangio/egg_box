#include <SID.h>

/************************************************************************

        Arduino controls an
	Atmega8 MOS6581 SID Emulator
        
	( SID = Sound Interface Device )

        Taken from c/64 user manual


***************************************************************************/



SID mySid;

void setup() {
  Serial.begin(9600);
  digitalWrite(13, HIGH); //turn on debugging LED
  mySid.begin();
}

  
  /************************************************************************
	
    void set_frequency(uint16_t pitch,uint8_t channel)
    
    Set sid frequncy registers.
    
    The pitch has to be:
    
    pitch=16.77*frequency
    
    The SID has 3 channels, which should be accesed according to this
    definition:
    
    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14
    
************************************************************************/

#define CHANNEL1  0
#define CHANNEL2  7
#define CHANNEL3  14


void play(byte hf, byte lf, byte dr){
  
  mySid.set_register(1,hf);
  mySid.set_register(0,lf);
  // CONTROL REGISTER = 4
  
  Serial.print(F("HF/LF:"));
  Serial.print(hf*256+lf);
  Serial.println("");
  mySid.set_register(4,33);
  delay(dr*2);
  mySid.set_register(4,32);
}

void play(byte hf, byte lf, byte dr,byte hf1, byte lf1, byte dr1){
  play(hf,lf,dr);
  play(hf1,lf1,dr1);
}

/************************************************************************
	
    main loop
    
************************************************************************/        
void loop()
{
  uint16_t n;
  uint8_t flag,k;
  uint16_t soundindex=0;
  
  for(byte s=0; s<=24;s++){
    mySid.set_register(s,0);
  }
  
  // Attack/decay
  
  mySid.set_register(5,9);
  //mySid.set_register(5,190);
  // SUSTAIN/RELEASE
  mySid.set_register(6,9);
  
  
  // Violino (per dire)
  //mySid.set_register(5,88);
  //mySid.set_register(6,89);
 
  // Piano D=9, il resto zero
  //mySid.set_register(5,9);
  //mySid.set_register(6,0);
  
  mySid.set_register(24,15); // SET MAX VOLUME
  
 
  
  // READ HF,LF,DR
  play(25,177,250);
  play(28,214,250);
  play(25,177,250,25,177,250);

  play( 25,177,125,28,214,125);
 play( 32,94,750,25,177,250);
 play( 28,214,250,19,63,250);
 play( 19,63,250,19,63,250);
 play( 21,154,63,24,63,63);
 play( 25,177,250,24,63,125);
 play( 19,63,250);
 Serial.println("SID STOPS HERE");
  //while(1); // do nothing and loop forever
}
