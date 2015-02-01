
int notes[]={ 262,294,330,349};
int sensorValue, sensorLow=1023, sensorHigh=0;
void setup() {
 Serial.begin(9600);

 pinMode(13,OUTPUT);
 digitalWrite(13,HIGH);
 while(millis() <= 2000){
    sensorValue=analogRead(A5);
    if(sensorValue > sensorHigh) {
      sensorHigh=sensorValue;
    }
    
    if(sensorValue < sensorLow){
      sensorLow=sensorValue;
    }
 }
 
 digitalWrite(13,LOW);
 Serial.print("SR:");
 Serial.print(sensorLow);
 Serial.print(" ");
 Serial.println(sensorHigh);
}

const auto keyboardTime=300;

void loop() {
  
  // Until a button is pressed, play theremin mode
  int keyval=analogRead(A0);
  
  if(keyval==0){
    
    //Theremin mode
    sensorValue=analogRead(A5);
    // Intead of 50,3500
    int pitch=map(sensorValue, sensorLow,sensorHigh, 0, 2500);
    tone(8,pitch,20);
    delay(10);
    //delay(keyboardTime);
  }
    // Music mode
   noTone(8);
   //Serial.println(keyval);
      if(keyval==1023){
        //Serial.print("note");
        tone(8, notes[0],keyboardTime);
      }else if (keyval >= 990 && keyval <= 1010 ){
        tone(8,notes[1],keyboardTime);
      }else if (keyval >=505 && keyval <=515){
        tone(8,notes[2], keyboardTime);
      }else if (keyval >=5 && keyval <=10){
        tone(8,notes[3],keyboardTime);
      }else {
        noTone(8);
      }
      if(keyval >1) { delay(keyboardTime); }
  
  

}
