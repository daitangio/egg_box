
int notes[]={ 262,294,330,349};
void setup() {
 Serial.begin(9600);

}

void loop() {
  int keyval=analogRead(A0);
  Serial.println(keyval);
  if(keyval==1023){
    //Serial.print("note");
    tone(8, notes[0]);
  }else if (keyval >= 990 && keyval <= 1010 ){
    tone(8,notes[1]);
  }else if (keyval >=505 && keyval <=515){
    tone(8,notes[2]);
  }else if (keyval >=5 && keyval <=10){
    tone(8,notes[3]);
  }else {
    noTone(8);
  }

}
