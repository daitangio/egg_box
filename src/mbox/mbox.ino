// Erlng-like mbox system.


#include <NilRTOS.h>

// Use tiny unbuffered NilRTOS NilSerial library.
#include <NilSerial.h>
#include <NilFIFO.h>

const uint8_t LED_PIN = 13;

// Macro to redefine Serial as NilSerial to save RAM.
// Remove definition to use standard Arduino Serial.
#define Serial NilSerial


// API 16bit atoms
enum Atom : word {
  ok, error,on, off
};



//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waLedServer, 64);
// Declare ALSO the MBOX of one element
NilStatsFIFO<Atom, 4> LedServerMBOX;

// Declare thread function for thread 1.
// it is an RPC high-level server
NIL_THREAD(LedServer, arg) {

  while (TRUE) {
    
    Atom* actionPtr=LedServerMBOX.waitData(TIME_INFINITE);
    if(!actionPtr) continue; // No data continue
    
    Atom action=*actionPtr;
    //Serial.println("Action!");
    switch(action){
      case on :
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LedServer on  requested");
        break;
      case off:
        digitalWrite(LED_PIN, LOW);
        Serial.println("LedServer off requested");
        break;
      default:
        Serial.println("LedServer Unknown request:");
        Serial.print((word)action);
        Serial.println("");
        break;
    }
    LedServerMBOX.signalFree();
    // Sleep so lower priority threads can execute.
    nilThdSleep(500);
  }
}
//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waThread2, 64);

// Declare thread function for thread 2.
// This thread "uses" the led guy
/** Original rlang API
rpc(Pid, Query) ->
 Ref = make_ref(),
 Pid ! {self(), Ref, Query},
 %% watch this space **************
 receive
 {Ref, Response} ->
 Response
 end.
 */
 
NIL_THREAD(Thread2, arg) {

  while (TRUE) {
    
    // Sleep so lower priority threads can execute.
    nilThdSleep(100);
    Atom* freeSlot=LedServerMBOX.waitFree(TIME_INFINITE);
    // Continue if no free space.
    //if (freeSlot == 0) continue;
    Serial.print("Sending on request");
    *freeSlot=on; 
    LedServerMBOX.signalData();
    
    nilThdSleep(400);    
    Serial.print("Sending off request");
    freeSlot=LedServerMBOX.waitFree(TIME_INFINITE);
    *freeSlot=off;
    LedServerMBOX.signalData();


    
    Serial.print("Sending ok request1");
    freeSlot=LedServerMBOX.waitFree(TIME_INFINITE);
    *freeSlot=ok;
    LedServerMBOX.signalData();

    Serial.print("Sending 3 fast ok request");
    freeSlot=LedServerMBOX.waitFree(TIME_INFINITE);
    *freeSlot=ok;
    LedServerMBOX.signalData();
    freeSlot=LedServerMBOX.waitFree(TIME_INFINITE);
    *freeSlot=ok;
    LedServerMBOX.signalData();
    freeSlot=LedServerMBOX.waitFree(TIME_INFINITE);
    *freeSlot=ok;
    LedServerMBOX.signalData();
    Serial.print("Sleeping a bit");
    nilThdSleep(2000);
  }
}
//------------------------------------------------------------------------------
/*
 * Threads static table, one entry per thread.  A thread's priority is
 * determined by its position in the table with highest priority first.
 *
 * These threads start with a null argument.  A thread's name is also
 * null to save RAM since the name is currently not used.
 */
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, LedServer, NULL, waLedServer, sizeof(waLedServer))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_END()
//------------------------------------------------------------------------------
void setup() {

  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  // start kernel
  nilSysBegin();
}
//------------------------------------------------------------------------------
// Loop is the idle thread.  The idle thread must not invoke any
// kernel primitive able to change its state to not runnable.
void loop() {
  nilPrintStackSizes(&Serial);
  nilPrintUnusedStack(&Serial);
  Serial.println();
  LedServerMBOX.printStats(&Serial);

  // Delay for one second.
  // Must not sleep in loop so use nilThdDelayMilliseconds().
  // Arduino delay() can also be used in loop().
  nilThdDelayMilliseconds(1000);
}

