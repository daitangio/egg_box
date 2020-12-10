/*
 * GG Collab example thread
 */
#include <NilRTOS.h>
#include "nil.h"

// The LED is attached to pin 13 on Arduino.
const uint8_t LED_PIN = 13;

void yield(){
  Serial.print(".");
  // Collab switch
  NIL_IRQ_PROLOGUE();
  nilSysTimerHandlerI();
  NIL_IRQ_EPILOGUE();
}
//------------------------------------------------------------------------------
/*
 * Thread 1, turn the LED off 
 */
// Declare a stack with 128 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waThread1, 128);

// Declare the thread function for thread 1.
NIL_THREAD(Thread1, arg) {
  while (TRUE) {
       
    // Turn LED off.
    digitalWrite(LED_PIN, LOW);
    nilThdSleepMilliseconds(500);
    yield();
  }
}
//------------------------------------------------------------------------------
/*
 * Thread 2, turn the LED on and signal thread 1 to turn the LED off.
 */
// Declare a stack with 128 bytes beyond context switch and interrupt needs. 
NIL_WORKING_AREA(waThread2, 128);

// Declare the thread function for thread 2.
NIL_THREAD(Thread2, arg) {

 
  while (TRUE) {
    // Turn LED on.
    digitalWrite(LED_PIN, HIGH);
    
    // Sleep for 500 milliseconds.

    nilThdSleepMilliseconds(500);
    yield();
  }
}
//------------------------------------------------------------------------------
/*
 * Threads static table, one entry per thread.  A thread's priority is
 * determined by its position in the table with highest priority first.
 * 
 * These threads start with a null argument.  A thread's name may also
 * be null to save RAM since the name is currently not used.
 */
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY("thread1", Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY("thread2", Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_END()
//------------------------------------------------------------------------------
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  // Start Nil RTOS.
  nilSysBegin();
}
//------------------------------------------------------------------------------
// Loop is the idle thread.  The idle thread must not invoke any 
// kernel primitive able to change its state to not runnable.
void loop() {
  yield();
}
