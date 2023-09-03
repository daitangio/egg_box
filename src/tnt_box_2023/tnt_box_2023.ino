/*
  Blink evolution is here!
  The system will sleep until you press the button on pin wakeupButton
  Then a magic led dance will start

Schematics: 
  Connect pin 2 -> Button -> GND

  3 Leds on Pin
  TBD 12,11,10

  Operative Leds to ?

  LCD TO pin 20,21 for I2C communicartion


Interaction with LCD is very delicate: AVOID printing inside interrupt

Feature:
+ LCD Logging
+ 2-lines message display with scrolling


 */

// #define DEBUG_LOG yeppa
#define LCD_DEBUG_LOG yeppa

#include <jj-log.h>
#include <avr/sleep.h>
#include "mem_free.h"

// CHECK ON MEGA The I2C pins on the board are 20 and 21.
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4); // set the LCD address to 0x27 for a 20 chars and 4 line display


// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int aliveLed = 13;
const int wakeupButton = 2; // MEGA PINS for interrupt  are 2, 3, 18, 19, 20, 21
#ifdef LCD_DEBUG_LOG
const int DelayTime = 33;  // Lcd debug will slow down a lot
#else
const int DelayTime = 44;   // 44 is good
#endif
/////////////////////////////////////////////////////////////////
char runningMode = 1; // 1 = Start running, 0 = Start Sleeping
/////////////////////////////////////////////////////////////////


int stripeLeds[] = { 12, 13,   9  };
const int stripeLength=sizeof(stripeLeds)/sizeof(stripeLeds[0]);

#include <Eventually.h>
EvtManager mgr;





const int dual_mode_period=5;
/**
 * LCD Status message printing
 * Also support a minimal scrolling mechianics
 * It is a dual mode lcd
 */
inline void sayLcdMsg(String str){
  #ifdef LCD_DEBUG_LOG
  static int8_t super_counter=1;
  static String prevMsg1="";
  static String prevMsg2="";

  // Clearing lcd is a slow procedure but because we need to scroll all display it seems a good move
  // to reduce code size and also increment speed
  lcd.clear();
  if ((super_counter % dual_mode_period) == 0) {
    lcd.setCursor(0,1);
    lcd.print(F("Tnt Box 2023"));
    lcd.print( (float)(millis() / 1000));
    lcd.print(F("s"));
    lcd.setCursor(0,2);    
    String memFree=String(F("Mem:"));
    memFree.concat(freeMemory());  
    lcd.print(memFree);

    lcd.setCursor(0,3);
    lcd.print(F("by Giovanni Giorgi"));
  }else {

    String finalMsg="";
    finalMsg.concat(super_counter);
    finalMsg.concat(" ");
    finalMsg.concat(str);

    
    lcd.setCursor(0,3); 
    lcd.print(finalMsg);  

    // Scrolling logic:
    
    lcd.setCursor(0,2);
    lcd.print(prevMsg1);

    
    lcd.setCursor(0,1);
    lcd.print(prevMsg2);

    prevMsg2=prevMsg1;
    prevMsg1=finalMsg;
    // Print mem free on top

    lcd.setCursor(0,0);
    String memFree=String(F("v1.2.2 Mem:"));
    memFree.concat(freeMemory());
    lcd.print(memFree);
  
  }

 

  // Ensure super counter value never exceed 99
  super_counter= (super_counter+1) % 100;


  #else
    return;
  #endif;
  
}

// Use String constructor to "eat" static strings
#define say(c)      sayLcdMsg(String(F(c)));


// debug will be defined as empty function when debug mode is off
#define debug(c)    sayLcdMsg(String(F(c)));


///// Sleep Mechianics
void sleepModeCheckCallback()
{
  if (runningMode == 1)
  {
    runningMode = 0;
    l("* Requesting Sleep...");
  }
  // Disable interrupt to be one time fire
  detachInterrupt(digitalPinToInterrupt(wakeupButton));
}


void wakeUp(){
  // lcd.backlight();
  l("* COOOME BACK !");
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(wakeupButton));
}

void lcdInit(){
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("2021 LD v1.1 ");
  lcd.print(stripeLength);
  
  say("Ready.");
}





/** Retrun true on sleep detect */
inline bool checkForSleep(){
  if (runningMode == 0){
    /* For the sleep mode refer to https://thekurks.net/blog/2018/1/24/guide-to-arduino-sleep-mode
     * IT IS VERY VERY IMPORTANT to have some delay to let stabilize it
     * Also it is unclear if stack is correctly preserved after wakeup (it seems)
     */
    l("...Sleeping...");
    
    // Remove old interrupts if any
    detachInterrupt(digitalPinToInterrupt(wakeupButton));

    say("...Sleeping...");    
    delay(200);

    delay(340);

    // Turn off all
    for(int i=0; i<stripeLength; i++) {
        analogWrite(stripeLeds[i], 0 );
    }
    lcd.noBacklight();



    sleep_enable();
    attachInterrupt(digitalPinToInterrupt(wakeupButton), wakeUp, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    delay(1000);
    sleep_cpu();
    l("WAAAKE UP2 Setting up sleep interrupt again");
    // Try to reinit lcd and show stuff    
    runningMode = 1;
    delay(400);

    lcdInit();    
    
    attachInterrupt(digitalPinToInterrupt(wakeupButton), sleepModeCheckCallback, CHANGE);    
    return true;
  }else{
    return false;
  }
  
}

/** Collaborative delay function.
 *  Make a double check for giving
 */
inline void my_delay(int millis) {
  if(millis > 10 ){
    checkForSleep();
    delay(millis/2);
    checkForSleep();
    delay(millis/2);
  }else{
    delay(millis);
  }
}

void loopold()
{
  // We can start in any sleep state and all will work

  // Fade cycle
  if (runningMode == 1 /** Running? */)
  {   
    
    const int minBright = 0;    
    const int fadeAmount = 64; // how many points to fade the LED by
    const int maxBright = 255 ;


    int brightness = minBright; // how bright the LED is
    
    
    for(int dimezzer=1; dimezzer <= 64; dimezzer*=2 ){
      for(int i=0; i<stripeLength; i++) {
        switch(i){
          case 0:
            brightness=64  / dimezzer ;
            break;
          case 1:
            brightness=128 / dimezzer ;
            break;
          case 2:
            brightness=256 / dimezzer;
            break;
          default:
            brightness=0;
            break;
          
        }
        analogWrite(stripeLeds[i], brightness);
        my_delay(DelayTime);      
      }
    }

    for(int expander=64; expander >1; expander/=2 ){
      for(int i=0; i<stripeLength; i++) {
        switch(i){
          case 0:
            brightness=1 * expander ;
            break;
          case 1:
            brightness=3 * expander ;
            break;
          case 2:
            brightness=4 * expander ;
            break;
          default:
            brightness=0;
            break;
          
        }
        analogWrite(stripeLeds[i], brightness);
        my_delay(DelayTime);      
      }
    }

    
    say("Cycle ends");


    // We must sleep ?
    checkForSleep();
    
  }
}


bool blink1() {
  static bool state=LOW;
  state = !state; // Switch light states
  digitalWrite( stripeLeds[0], state); // Display the state
  digitalWrite( stripeLeds[1], state); // Display the state
  digitalWrite( stripeLeds[2], state); // Display the state
  return false; // Allow the event chain to continue
}

bool checkForSleep2(){
  if(checkForSleep()==false){
    say("No sleep yet");
  }
  return false;
}

bool statusMessages(){
  String m=F("Run Stats:");
  m.concat((float)(millis() / 1000));
  sayLcdMsg(m);
  return false;
}

//////////////////////////////////////////////////////////////////////////////
// the setup routine runs once when you press reset:
void setup()
{
  lcdInit();
  // initialize the digital pin as an output.
  pinMode(aliveLed, OUTPUT);
  pinMode(wakeupButton, INPUT_PULLUP);

  for(int i=0; i<stripeLength; i++) {
    pinMode(stripeLeds[i], OUTPUT);
    analogWrite(stripeLeds[i], 0 );
  }

  if(runningMode == 1){
    // if the initial state is 1 it means we are not sleeping so we must enable the interrupt
    attachInterrupt(digitalPinToInterrupt(wakeupButton), sleepModeCheckCallback, CHANGE);
  }

  mgr.addListener(new EvtTimeListener(44, true,   (EvtAction)blink1)                 );
  mgr.addListener(new EvtTimeListener(88, true,   (EvtAction)blink1)                 );
  mgr.addListener(new EvtTimeListener(900, true,   (EvtAction)checkForSleep2)                 );
  //mgr.addListener(new EvtTimeListener(5000, true,   (EvtAction)statusMessages)                 );

  #ifdef DEBUG_LOG
    Serial.begin(9600);
    l("2021 Led Dreams");
  #endif    
}
USE_EVENTUALLY_LOOP(mgr) // Use this instead of your loop() function.



// Local variables:
// mode:c++
// mode:company
// End:

