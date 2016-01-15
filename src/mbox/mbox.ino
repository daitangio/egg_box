// Erlng-like mbox system.


#include <NilRTOS.h>

// Use tiny unbuffered NilRTOS NilSerial library.
#include <NilSerial.h>

const uint8_t LED_PIN = 13;

// Macro to redefine Serial as NilSerial to save RAM.
// Remove definition to use standard Arduino Serial.
#define Serial NilSerial
#include "mbox.h"

////////////////////////////////// DATA DICTIONARY
// API word: 16bit atoms (32bit are unsigned long)
enum Atom : unsigned long {
    ok, error,unused, // SPECIAL MESSAGES
        on, off, // Standard protocol messagges
        walkerTexasRanger, // Special protocol messages
        thread2,ledServer, // Processes (pids)
        led13=13           // Params
};
///^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ DATA DICTIONARY




//////////////////////////////////////////
/**** MSG SPECIALIZED API ******/
typedef struct /*Msg*/ {
  Atom caller;
  Atom k;
  Atom v;
} Msg;



/** Send to pid a simple message  */
void send2Pid(Atom dest, Atom send_caller, Atom k){
  send2Pid(dest,send_caller,k,unused);
  
}

/** Return 0 on fail
 *  1 on success
  */
int send2Pid(Atom dest, Atom sender_caller, Atom k, Atom v)
{
  Msg m;
  m.caller=sender_caller;
  m.k=k;
  m.v=v;

  /*** Put here the list of all static server */
  switch(dest){
  case ledServer:
      LedServerMBOX.send(m);
      break;
  case thread2:
      Thread2MBOX.send(m);
      break;
  default:
      return 0;
      break;
  }
  return 1;
}



  
// Example of a Msg Led Server
// One for the command, the second for the parameter
NilStatsFIFO<Msg, 10> LedServerMBOX;
//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waLedServer, 64);

// Declare thread function for thread 1.
// it is an RPC high-level server
NIL_THREAD(LedServer, arg) {

  while (TRUE) {

    Msg* p=LedServerMBOX.receive(); // Blocks
    Msg  bip=*p;
    Atom    action=p->k, param=p->v, caller = p->caller;
    
    switch(action){
      case on :
        digitalWrite((uint8_t) param, HIGH);
        Serial.println("LedServer on "+itoa((uint8_t)param));
        // Signal all ok
        send(caller,ledServer,ok,param);
        break;
      case off:
        digitalWrite(param,LOW);
        break;
      default:
        Serial.println("LedServer Unknown request:");
        Serial.print((word)action);
        Serial.println("");
        break;
    }
    // ??? Sleep so lower priority threads can execute.
    nilThdSleep(100);
  }
}
//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waThread2, 64);

// Declare thread function for thread 2.
// This thread "uses" the led guy
/** Original erlang API
rpc(Pid, Query) ->
 Ref = make_ref(),
 Pid ! {self(), Ref, Query},
 %% watch this space **************
 receive
 {Ref, Response} ->
 Response
 end.
 */


NilStatsFIFO<Msg, 10> Thread2MBOX;
NIL_THREAD(Thread2, arg) {

  while (TRUE) {
      /**** TODO: 
       * Send le d on request to ledServer and wait for ok to response
       */
    Serial.print("Sending on request (async)");
    // Pid ! {self(), Ref, Query},
    send(ledServer,thread2, on ,led13);
    // %% watch this space **************
    Msg* p=Thread2MBOX.receive(); // Blocks
    Atom    action=p->k, param=p->v, caller = p->caller;
    //TODO: do Better then a double switch
    switch(caller){
    case ledServer:
        switch(action){
        case ok:
            // ok proceed 
            break;
        }
        break;
    }
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

// Local variables:
// mode:c++
// End:
