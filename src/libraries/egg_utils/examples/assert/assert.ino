/** Super simple assert usage example
  */

/** comment DEBUG_ASSERT to avoid debugging */
#define DEBUG_ASSERT 1
#include <jj-assert.h>

#define DEBUG_LOG
#include <jj-log.h>


void setup() {
	say("Ready to rock");
	jjAssert(1==1);
	describe(7+3);
	Serial.print("This will fail:");
	jjAssert(1+1==4);
}

void loop(){
	
}