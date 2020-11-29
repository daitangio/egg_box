/*
  Blink evolution is here!
  The system will sleep until you press the button on pin wakeupButton
  Then a magic led dance will start

Schematics: 
Connet pin 2 -> Button -> GND

3 Leds on Pin
TBD 12,11,10

Operative Leds to ?
LCD TO ?


 */

#define DEBUG_LOG yeppa
#include <jj-log.h>

#include <avr/sleep.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int aliveLed = 13;
const int wakeupButton = 2; // MEGA PINS are 2, 3, 18, 19, 20, 21
const int DelayTime = 44;   // 44 is good
/////////////////////////////////////////////////////////////////
char sleepMode = 1; // 1 = Start running, 0 = Start Sleeping
/////////////////////////////////////////////////////////////////



void sleepModeRequested()
{
  if (sleepMode == 1)
  {
    sleepMode = 0;
    l("* Requesting Sleep...");
  }
  // Disable interrupt to be one time fire
  detachInterrupt(digitalPinToInterrupt(wakeupButton));
}


void wakeUp(){
  l("* COOOME BACK !");
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(wakeupButton));
}

// the setup routine runs once when you press reset:
void setup()
{
  // initialize the digital pin as an output.
  pinMode(aliveLed, OUTPUT);
  pinMode(wakeupButton, INPUT_PULLUP);

  if(sleepMode == 1){
    // if the initial state is 1 it means we are not sleeping so we must enable the interrupt
    attachInterrupt(digitalPinToInterrupt(wakeupButton), sleepModeRequested, CHANGE);
  }

  #ifdef DEBUG_LOG
  Serial.begin(9600);
  l("2021 Led Dreams");
  #endif    
}


inline void checkForSleep(){
  if (sleepMode == 0){
    /* For the sleep mode refer to https://thekurks.net/blog/2018/1/24/guide-to-arduino-sleep-mode
     * IT IS VERY VERY IMPORTANT to have some delay to let stabilize it
     * Also it is unclear if stack is correctly preserved after wakeup (it seems)
     */
    l("...Sleeping...");
    // Remove old interrupts if any
    detachInterrupt(digitalPinToInterrupt(wakeupButton));
    delay(DelayTime * 10);
    sleep_enable();
    attachInterrupt(digitalPinToInterrupt(wakeupButton), wakeUp, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    delay(1000);
    sleep_cpu();
    l("WAAAKE UP2 Setting up sleep interrupt again");
    sleepMode = 1;
    delay(400);
    attachInterrupt(digitalPinToInterrupt(wakeupButton), sleepModeRequested, CHANGE);    
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

void loop()
{
  // We can start in any sleep state and all will work

  // Fade cycle
  if (sleepMode != 0)
  {   


    const int minBright = 0;
    const int maxBright = 128;
    const int fadeAmount = 4; // how many points to fade the LED by

    int pin = aliveLed;
    int brightness = minBright; // how bright the LED is

    analogWrite(pin, brightness);
    // fade in,,,,
    while (brightness < maxBright)
    {
      brightness = brightness + fadeAmount;
      analogWrite(pin, brightness);      
      my_delay(DelayTime);
    }
  
    // fad out...
    while (brightness > minBright)
    {
      brightness = brightness - fadeAmount;
      analogWrite(pin, brightness);
      my_delay(DelayTime);
    }
    // extra delay after fadeout
    my_delay(10 * DelayTime);
  }else{
    checkForSleep();
  }
}




// Local variables:
// mode:c++
// mode:company
// End:

