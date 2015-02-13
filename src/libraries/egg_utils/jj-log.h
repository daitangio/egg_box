
#ifndef JJ_LOG_H
#define JJ_LOG_H

#ifdef DEBUG_LOG


// Ensure Serial and other stuff is  in:
#include <Arduino.h>

// Ensure F() macro is in
#include <WString.h>
// As F is defined...
// #define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))


// General purpose function to save memory: works only on static messages
// 212 bytes vs 224 bytes consumed of global dynamic memory
//#define p(x)	Serial.print(F(x));

// VERSION TO COMPARE SAVINGS: #define p(x)	Serial.print(x);


inline void jj_do_debug_log(String f,int line, String msg){
	Serial.print( F(" [L] ") );
        Serial.print(f); Serial.print(F(":"));
	Serial.print(line);
	Serial.print( F("\t") );
	Serial.println(msg);		
}


// Macro and template can be combined to get static code
template<class exprType> inline void jj_do_debug_describe(int line, String expr, exprType result ){
	Serial.print( F(" [A] ") );
	Serial.print(line);
	Serial.print( F(" ") );
	Serial.print(expr);
	Serial.print( F(" = ") );
	Serial.print(result);
}

// Use of F() function ensure string are allocated in the code area.
// With macro and inline template this optimization strive to obtain the maximum from the guy
//

#define l(c)      jj_do_debug_log(__FILE__,__LINE__, F(c));
#define describe(c) jj_do_debug_describe(__LINE__, F( #c ) , c );




#else

#define l(c)      /*Nothing*/ 
#define describe(c) /*Nothing*/

#warning  "----------------------"
#warning  "|   LOGGING DISABLED  |"
#warning  "----------------------"

#endif

#endif
