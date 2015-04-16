#include <avr/pgmspace.h>

/**
  * Generate 
  * midicsv ../../midi/midicsv-1.1/empire.mid  | sort -t ',' -k 2 -n  | more
  
  */
const char empireTest[] PROGMEM ={
"10, 768, Note_on_c, 8, 55, 98\0"
"12, 768, Note_on_c, 11, 31, 115\0"
"12, 768, Note_on_c, 11, 43, 109\0"
"12, 768, Note_on_c, 11, 55, 116\0"
"15, 768, Note_on_c, 9, 38, 60\0"
"10, 782, Note_off_c, 8, 55, 98\0"
"15, 784, Note_off_c, 9, 38, 60\0"
"12, 786, Note_off_c, 11, 31, 115\0"
"12, 786, Note_off_c, 11, 43, 109\0"
"12, 799, Note_off_c, 11, 55, 116\0"
"12, 959, Note_on_c, 11, 31, 110\0"
"12, 959, Note_on_c, 11, 43, 106\0"
"12, 959, Note_on_c, 11, 55, 110\0"
"10, 961, Note_on_c, 8, 55, 93\0"
"15, 962, Note_on_c, 9, 38, 61\0"
"10, 974, Note_off_c, 8, 55, 93\0"
"12, 974, Note_off_c, 11, 31, 110\0"
"12, 974, Note_off_c, 11, 43, 106\0"
"12, 977, Note_off_c, 11, 55, 110\0"
"15, 980, Note_off_c, 9, 38, 61\0"
"12, 1052, Note_on_c, 11, 31, 116\0"
"12, 1052, Note_on_c, 11, 43, 113\0"
"12, 1056, Note_on_c, 11, 55, 108\0"
"10, 1058, Note_on_c, 8, 55, 96\0"
"15, 1060, Note_on_c, 9, 38, 57\0"
"12, 1068, Note_off_c, 11, 31, 116\0"


};

char buffer[200];
const int MAX_COLUMNS=10;
String parsedData[MAX_COLUMNS];

// midi time counter:
long megaTimeCounter=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  Serial.println("");
  Serial.println("");
  Serial.println(F("Checking PRGMEM..."));
  
  // Check PRGMEM...
  int p=0;
  int lp=0;
  int parsedDataIndex=0;
  do {
    
    char cc=pgm_read_byte_near(empireTest+p);
    Serial.print(cc);
    buffer[lp]=cc;
    lp++;
    
    if(cc==',' || cc=='\0'){
      buffer[lp]='\0';
      String currentColumn=String(buffer);
      currentColumn.trim();
      parsedData[parsedDataIndex++]= currentColumn;
      lp=0;
    }
    
    if(cc=='\0'){ 
      Serial.println("");     
      Serial.println(F("Ready to process Line"));
      // If 2 is Note_on_c or Note_off_c we take action
      // if the master timing is friend with us
      for(int idx=0; idx < parsedDataIndex; idx++){
        Serial.print(idx);
        Serial.print(")");
        Serial.println(parsedData[idx]);
      }
      Serial.println("--");
      lp=0;
      parsedDataIndex=0;
    }
    
    if( (parsedDataIndex+1)==MAX_COLUMNS){
      Serial.println(F("UNPARSABLE CURRENT LINE. RESETTING."));
      lp=0;
      parsedDataIndex=0;
    }
    
    p++;
    if(p>sizeof(empireTest)){
      Serial.println(F("END OF PARSING"));
      break;
    }
  }while(true);
}

void loop() {
  // put your main code here, to run repeatedly:

}
