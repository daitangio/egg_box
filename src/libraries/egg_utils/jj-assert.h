
#ifdef DEBUG_ASSERT

#define jjAssert(c) jjAssert2(c, #c)

#define jjAssert2(c, m) {         \
  if (!(c)) {                      \
    Serial.print(F(m));            \
    Serial.print(", at line ");    \
    Serial.println(__LINE__);      \
    Serial.println(" FAILED");     \
  }                                \
}

#else

#define jjAssert(c) /*Nothing*/

#warning  "----------------------"
#warning  "|   ASSERT DISABLED  |"
#warning  "----------------------"

#endif
