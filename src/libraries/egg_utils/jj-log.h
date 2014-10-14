
#ifndef JJ_LOG_H
#define JJ_LOG_H

#ifdef DEBUG_LOG

// Ensure Serial and other stuff is  in:
#include <Arduino.h>

// Ensure F() macro is in
#include <WString.h>

inline void jj_do_debug_log(int line, String msg){
	Serial.print(" [LOG] ");
	Serial.print(line);
	Serial.print(" ");
	Serial.println(msg);		
}


// Macro and template can be combined to get static code
template<class exprType> inline void jj_do_debug_describe(int line, String expr, exprType result ){
	Serial.print(" [LOG] ");
	Serial.print(line);
	Serial.print(" ");
	Serial.print(expr);
	Serial.print(" = ");
	Serial.print(result);
}

// Use of F() function ensure string are allocated in the code area.
// With macro and inline template this optimization strive to obtain the maximum from the guy
//

#define say(c)      jj_do_debug_log(__LINE__, F(c));
#define describe(c) jj_do_debug_describe(__LINE__, F( #c ) , c );



#else

#define say(c)      /*Nothing*/ 
#define describe(c) /*Nothing*/

#warning  "----------------------"
#warning  "|   LOGGING DISABLED  |"
#warning  "----------------------"

#endif

#endif