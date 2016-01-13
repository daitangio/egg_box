// Erlng-like mbox system.


#include <NilRTOS.h>

// Use tiny unbuffered NilRTOS NilSerial library.
#include <NilSerial.h>

const uint8_t LED_PIN = 13;

// Macro to redefine Serial as NilSerial to save RAM.
// Remove definition to use standard Arduino Serial.
#define Serial NilSerial

//------------------------------------------------------------------------------
 /**
  * @class NilStatsFIFO
  * \brief A thread safe FIFO with statistics.
  *
  * A FIFO for a single producer/consumer pair with statistics.
  *
  */
template<typename Type, size_t Size>
class NilStatsFIFO {
 public:
  /** constructor */
  NilStatsFIFO(Type biMessageIntroducer) : _head(0), _maxOverrun(0),
    _minFree(Size), _overrun(0), _tail(0) {
    _preBiMessage=biMessageIntroducer;
    nilSemInit(&_dataSem, 0);
    nilSemInit(&_freeSem, Size);
  }
  /** Increment overrun count
   * @note Must only be called in producer thread.
   */
  void countOverrun() {
    if (_overrun == (size_t)-1) return;
    if (++_overrun > _maxOverrun) _maxOverrun = _overrun;
  }
  /** @return count of data records in the FIFO. */
  size_t dataCount() {return nilSemGetCounter(&_dataSem);}
  
  /** @return count of free records in the FIFO. */
  size_t freeCount() {return nilSemGetCounter(&_freeSem);}
  
  /**
   * Count of overruns since last call to signalData().
   * @note Must only be called in producer thread.
   * @return overrun count.
   *
   */
  size_t overrunCount() {return _overrun;}
  
  /** @return maximum overrun count. */
  size_t maxOverrunCount() {
    nilSysLock();
    size_t rtn = _maxOverrun;
    nilSysUnlock();
    return rtn;
  }
  /** @return minimum free count. */
  size_t minimumFreeCount() {
    nilSysLock();
    size_t rtn = _minFree;
    nilSysUnlock();
    return rtn;
  }

  /**
   * Print FIFO statistics.
   *
   * @param[in] pr Print stream for the output.
   *
   * @note Must be called when consumer/producer are idle
   *       for consistent results.
   */
  void printStats(Print* pr) {
    pr->print(F("FIFO record count: "));
    pr->println(Size);
    pr->print(F("Minimum free count: "));
    pr->println(_minFree);
    if (_maxOverrun) {
      pr->println();
      pr->println(F("** overrun errors **"));
      pr->print(F("Maximum overrun count: "));
      pr->println(_maxOverrun);
    }
  }
  
  /** Signal that a data record is ready.
   * @note Must only be called in producer thread.
   */
  void signalData() {
    _overrun = 0;
    nilSemSignal(&_dataSem);
  }
  /** Signal that a record is free.
   * @note Must only be called in consumer thread.
   */
  void signalFree() {
    cnt_t cnt = freeCount();
    if (cnt < _minFree) _minFree = cnt;
    nilSemSignal(&_freeSem);
  }
  
  /**
   * Wait for a data record.
   * @param[in] time      the number of ticks before the operation timeouts,
   *                      the following special values are allowed:
   *                      - @a TIME_IMMEDIATE immediate timeout.
   *                      - @a TIME_INFINITE no timeout.
   *                      .
   * @note Must only be called in consumer thread.
   * @return pointer to the data record or null if no data record is availabile.
   */
  Type* waitData(systime_t time) {
    if (nilSemWaitTimeout(&_dataSem, time) != NIL_MSG_OK) {
      return 0;
    }
    Type* rtn = &_data[_tail];
    _tail = _tail < (Size - 1) ? _tail + 1 : 0;
    return rtn;
  }
  
  /**
   * Wait for a free record.
   * @param[in] time      the number of ticks before the operation timeouts,
   *                      the following special values are allowed:
   *                      - @a TIME_IMMEDIATE immediate timeout.
   *                      - @a TIME_INFINITE no timeout.
   *                      .
   * @note Must only be called in producer thread.
   * @return pointer to the free record or null if no free record is availabile.
   */
  Type* waitFree(systime_t time) {
    if (nilSemWaitTimeout(&_freeSem, time) != NIL_MSG_OK) {
      countOverrun();
      return 0;
    }
    Type* rtn = &_data[_head];
    _head = _head < (Size - 1) ? _head + 1 : 0;
    return rtn;
  }

 void send(Type toSend){
    Type* freeSlot=waitFree(TIME_IMMEDIATE);
    
    // Signal block
    if (freeSlot == 0) {
      Serial.println("NO FREEE STORE...BLOCKING");
      freeSlot=waitFree(TIME_INFINITE);
      Serial.println("UNBLOCKED");
    }
    //Serial.print("Sending request");
    *freeSlot=toSend; 
    signalData();
 }

  
  void send(Type v, Type p) {
    // Send a double guy using the preface
    send(_preBiMessage);
    send(v);
    send(p);
  }

  Type receive(){
    Atom* actionPtr=waitData(TIME_INFINITE);
    // if(!actionPtr) continue; // No data continue
    Atom action=*actionPtr;
    return action;
  }

  
  /** Blocking receive return the rest of the headmatch
   * or the _preBiMessage as error
   */
  Type receive(Type headMatch){
    Atom* actionPtr=waitData(TIME_INFINITE);
    Atom action=*actionPtr;
    if(action==_preBiMessage){
      Action header=(*waitData(TIME_INFINITE));
      if(header==headMatch){
        Atom toReturn=(*waitData(TIME_INFINITE));
        return toReturn;
      }
    }
    // Err
    return _preBiMessage;
  }
  
 /** Register a first-chance function for the given atom.
   * Put the message back in queue if fails
   */
 void onReceive(Type atom2Check, void (*myProcFun)(Type) ){
    Type* actionPtr=waitData(TIME_IMMEDIATE);
    if(!actionPtr) return; // No data return
    Type action=*actionPtr;
    if(action == atom2Check){
      myProcFun(atom2Check);
    }else{
      // put back data for further processing
      send(action);
    }
 }
 
 private:
  Type _data[Size];
  Type _preBiMessage;
  semaphore_t _dataSem;
  semaphore_t _freeSem;
  size_t _head;
  size_t _maxOverrun;
  size_t _minFree;
  size_t _overrun;
  size_t _tail;
};


// API word: 16bit atoms (32bit are unsigned long)
enum Atom : word {
    m2=0, /** Message with params (used for 2-pair messages) */
    ok, error,on, off, // Actions
    thread2, /* Placeholder */
    ledServer  
};


  
// Declare ALSO the MBOX of the led server
NilStatsFIFO<Atom, 10> LedServerMBOX(m2);
//? NilStatsFIFO<Msg, 10> LedServerMBOX;

//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waLedServer, 64);

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



NIL_THREAD(Thread2, arg) {

  while (TRUE) {
    
    // Sleep so lower priority threads can execute.
    nilThdSleep(100);
    Atom* freeSlot=LedServerMBOX.waitFree(TIME_INFINITE);
    // Continue if no free space.
    //if (freeSlot == 0) continue;
    Serial.print("Sending on request (async)");
    LedServerMBOX.send(on);    
    nilThdSleep(400);
    LedServerMBOX.send(off);
    LedServerMBOX.send(ok);
    LedServerMBOX.send(ok);
    LedServerMBOX.send(ok); 
    LedServerMBOX.send(ok);
    // Emulate Pid ! {self(), Ref, Query},
    // Emulate  receive
    
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
