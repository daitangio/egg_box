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
+ 2-lines message display


 */

// #define DEBUG_LOG yeppa
#include <jj-log.h>

#include <avr/sleep.h>


// CHECK ON MEGA The I2C pins on the board are 20 and 21.
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4); // set the LCD address to 0x27 for a 20 chars and 4 line display


// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int aliveLed = 13;
const int wakeupButton = 2; // MEGA PINS for interrupt  are 2, 3, 18, 19, 20, 21
const int DelayTime = 22;   // 44 is good
/////////////////////////////////////////////////////////////////
char runningMode = 1; // 1 = Start running, 0 = Start Sleeping
/////////////////////////////////////////////////////////////////
long super_counter=1;


/**
 * LCD Status message printing
 * Also support a minimal scrolling mechianics
 */
void sayLcdMsg(const __FlashStringHelper *str){
  static __FlashStringHelper *prevMsg=F("");
  lcd.setCursor(0,3);  
  lcd.print(str);

  lcd.setCursor(0,2);
  // Clear prev line
  lcd.print(F("                    "));
  lcd.setCursor(0,2);
  lcd.print(prevMsg);
  prevMsg=str;
}

#define say(c)      sayLcdMsg(F(c));


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
  lcd.print("** 2021 LD");
  
  say("Ready.");
}

//////////////////////////////////////////////////////////////////////////////
// the setup routine runs once when you press reset:
void setup()
{
  lcdInit();
  // initialize the digital pin as an output.
  pinMode(aliveLed, OUTPUT);
  pinMode(wakeupButton, INPUT_PULLUP);

  if(runningMode == 1){
    // if the initial state is 1 it means we are not sleeping so we must enable the interrupt
    attachInterrupt(digitalPinToInterrupt(wakeupButton), sleepModeCheckCallback, CHANGE);
  }

  #ifdef DEBUG_LOG
  Serial.begin(9600);
  l("2021 Led Dreams");
  #endif    
}



inline void checkForSleep(){
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

    delay(440);
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
  if (runningMode == 1 /** Running? */)
  {   

    say("Pulse Loop Start");
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
    say("Pulse Loop Mid");
    // fad out...
    while (brightness > minBright)
    {
      brightness = brightness - fadeAmount;
      analogWrite(pin, brightness);
      my_delay(DelayTime);
    }
    


    super_counter++;
    lcd.setCursor(14,0);
    lcd.print("[");
    lcd.print((long) super_counter );
    lcd.print("]");

    // We must sleep ?
    checkForSleep();
    
  }
}




// Local variables:
// mode:c++
// mode:company
// End:

