
/** The midi player tester via SID emulator
 * A0 -> Analog input for a pool of button attached with a resistence
 * 
 * Pin 9: Sid output (need amplifier for better result)
 */
#include <pitches_it.h>
//#include <NilRTOS.h>
#include <jj-log.h>

#include <SID.h>
SID mySid;

// To simplify prototiping, prefer constant to passing parameters to function
// all way around
const int speakerOut=9;  // Chnaged from 8 to 9 for sid sinth compatibility


//const int DelayTime=44; // 44 is good

///////////////////////////////
///////////////////////////////
void playNote(byte voice, uint16_t freq){
  //uint16_t freq=freqBase*20;
  byte low = freq & 0x00FF;
  byte high= freq >>8;
  mySid.set_register(voice+0,low);
  mySid.set_register(voice+1,high);

  Serial.print("Sid:");
  Serial.println(freq);
  /*Serial.print(freqBase);
  Serial.print(F(" Fn="));
  Serial.print(freq);
  Serial.print(high);
  Serial.print("/");
  Serial.print(low);
  Serial.println("");*/
  
  mySid.set_register(voice+4,33);
}

void stopSound(byte voice){
  mySid.set_register(voice+4,32);
  mySid.set_register(voice+0,0);
  mySid.set_register(voice+1,0);
}
///////////////////////////////
///////////////////////////////



/* Scala
 * Use midicsv  to convert the music and got the notes...
 * http://www.fourmilab.ch/webtools/midicsv/




0, 0, Header, 1, 2, 960
1, 0, Start_track
1, 0, Tempo, 500000
1, 0, Key_signature, 0, "major"
1, 0, Title_t, "testNote\000"
    1,    0, Time_signature,   4,     2,    24, 8
Track, Time, Time_signature, Num, Denom, Click, NotesQ
    The time signature, metronome click rate, and number of 32nd notes per MIDI quarter note (24 MIDI clock times) are given by the numeric arguments. Num gives the numerator of the time signature as specified on sheet music. Denom specifies the denominator as a negative power of two, for example 2 for a quarter note, 3 for an eighth note, etc. Click gives the number of MIDI clocks per metronome click, and NotesQ the number of 32nd notes in the nominal MIDI quarter note time of 24 clocks (8 for the default MIDI quarter note definition). 

1, 0, End_track
2, 0, Start_track
2, 0, Control_c, 0, 0, 0
2, 0, Control_c, 0, 32, 0
2, 0, Program_c, 0, 0
2, 0, Title_t, "Untitled\000"
2, 0, Control_c, 0, 7, 127

DO4 is 72
2,        0, Note_on_c, 0,         72, 80   

Track, Time, Note_on_c, Channel, Note, Velocity
Velocity (0 to 127). 
A Note_on_c event with Velocity zero is equivalent to a Note_off_c. 

There are 24 MIDI Clocks in every quarter note. 
(12 MIDI Clocks in an eighth note, 6 MIDI Clocks in a 16th, etc). 

2, 480, Note_off_c, 0, 72, 0
2, 480, Note_on_c, 0, 71, 80
2, 960, Note_off_c, 0, 71, 0
2, 960, Note_on_c, 0, 69, 80
2, 1440, Note_off_c, 0, 69, 0
2, 1440, Note_on_c, 0, 67, 80
2, 1920, Note_off_c, 0, 67, 0
2, 1920, Note_on_c, 0, 65, 80
2, 2400, Note_off_c, 0, 65, 0
2, 2400, Note_on_c, 0, 64, 80
2, 2880, Note_off_c, 0, 64, 0
2, 2880, Note_on_c, 0, 62, 80
2, 3360, Note_off_c, 0, 62, 0
2, 3360, Note_on_c, 0, 60, 80
2, 3840, Note_off_c, 0, 60, 0
2, 46079, Control_c, 0, 127, 0
2, 46079, End_track
0, 0, End_of_file

 
 */

/* ./midicsv  doremifasolasido.mid  | egrep ^2 | egrep Note_on_c |cut -d , -f 5,2
 */
const float scalaMusicale[]  ={ 
  NOTE_DO4,1,
  NOTE_RE4,1,
  NOTE_MI4,1,
  NOTE_FA4,1,
  NOTE_SOL4,1,
  NOTE_LA4,1,
  NOTE_SI4,1,
  NOTE_DO5,1,
  -1,-1,-1,-1 /** CLOSE MUSIC */
};

const int midiTest1[]= {
  0, 72,
  480, 71,
  960, 69,
  1440, 67,
  1920, 65,
  2400, 64,
  2880, 62,
  3360, 60,
  -1,-1,-1,-1
};

// Using PROGMEM is difficult
// See http://todbot.com/blog/2008/06/19/how-to-do-big-strings-in-arduino/

const int axelTest2[] /*PROGMEM*/  ={
  // File axel_foley-test.mid Track 2
 0, 65,
 960, 68,
 1680, 65,
 2160, 65,
 2400, 70,
 2880, 65,
 3360, 63,
 3840, 65,
 4800, 72,
 5520, 65,
 6000, 65,
 6240, 73,
 6720, 72,
 7200, 68,
 7680, 65,
 8160, 72,
 8640, 77,
 9120, 65,
 9360, 63,
 9840, 63,
 10080, 60,
 10560, 67,
 11040, 65,
 15360, 65,
 15360, 77,
 16320, 68,
 16320, 80,
 17040, 65,
 17040, 77,
 17520, 65,
 17520, 77,
 17760, 70,
 17760, 82,
 18240, 65,
 18240, 77,
 18720, 63,
 18720, 75,
 19200, 65,
 19200, 77,
 20160, 72,
 20160, 84,
 20880, 65,
 20880, 77,
 21360, 65,
 21360, 77,
 21600, 73,
 21600, 85,
 22080, 72,
 22080, 84,
 22560, 68,
 22560, 80,
 23040, 65,
 23040, 77,
 23520, 72,
 23520, 84,
 24000, 77,
 24000, 89,
 24480, 65,
 24480, 77,
 24720, 63,
 24720, 75,
 25200, 63,
 25200, 75,
 25440, 60,
 25440, 72,
 25920, 67,
 25920, 79,
 26400, 65,
 26400, 77,
 61440, 65,
 62400, 68,
 63120, 65,
 63600, 65,
 63840, 70,
 64320, 65,
 64800, 63,
 65280, 65,
 66240, 72,
 66960, 65,
 67440, 65,
 67680, 73,
 68160, 72,
 68640, 68,
 69120, 65,
 69600, 72,
 70080, 77,
 70560, 65,
 70800, 63,
 71280, 63,
 71520, 60,
 72000, 67,
 72480, 65,
 76800, 65,
 77760, 68,
 78480, 65,
 78960, 65,
 79200, 70,
 79680, 65,
 80160, 63,
 80640, 65,
 81600, 72,
 82320, 65,
 82800, 65,
 83040, 73,
 83520, 72,
 84000, 68,
 84480, 65,
 84960, 72,
 85440, 77,
 85920, 65,
 86160, 63,
 86640, 63,
 86880, 60,
 87360, 67,
 87840, 65,
 165120, 65,
 166080, 68,
 166800, 65,
 167280, 65,
 167520, 70,
 168000, 65,
 168480, 63,
 168960, 65,
 169920, 72,
 170640, 65,
 171120, 65,
 171360, 73,
 171840, 72,
 172320, 68,
 172800, 65,
 173280, 72,
 173760, 77,
 174240, 65,
 174480, 63,
 174960, 63,
 175200, 60,
 175680, 67,
 176160, 65,
 180480, 65,
 181440, 68,
 182160, 65,
 182640, 65,
 182880, 70,
 183360, 65,
 183840, 63,
 184320, 65,
 185280, 72,
 186000, 65,
 186480, 65,
 186720, 73,
 187200, 72,
 187680, 68,
 188160, 65,
 188640, 72,
 189120, 77,
 189600, 65,
 189840, 63,
 190320, 63,
 190560, 60,
 191040, 67,
 191520, 65,
 303360, 65,
 304320, 68,
 305040, 65,
 305520, 65,
 305760, 70,
 306240, 65,
 306720, 63,
 307200, 65,
 308160, 72,
 308880, 65,
 309360, 65,
 309600, 73,
 310080, 72,
 310560, 68,
 311040, 65,
 311520, 72,
 312000, 77,
 312480, 65,
 312720, 63,
 313200, 63,
 313440, 60,
 313920, 67,
 314400, 65,
 318720, 65,
 319680, 68,
 320400, 65,
 320880, 65,
 321120, 70,
 321600, 65,
 322080, 63,
 322560, 65,
 323520, 72,
 324240, 65,
 324720, 65,
 324960, 73,
 325440, 72,
 325920, 68,
 326400, 65,
 326880, 72,
 327360, 77,
 327840, 65,
 328080, 63,
 328560, 63,
 328800, 60,
 329280, 67,
 329760, 65,
  -1,-1,-1,-1
};

/*
  // see SID data sheet
  // http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet
  // Fout = (Fn * 0.0596) Hz
  // So to determine Fn (sid input we try...)
  */
int midi2FreqOLD(int midin);

uint16_t  midi2Sid(int midin){
  // NOTE_D03 --> 131
  // C3  130.81	2195
  //return midi2FreqOLD(midin)/0.0596;
  /*
 */
  switch(midin){
  case 60:return 0x122A;  // C4$	277.18	4650	122A 
  case 61:return 0x133F;
  case 62:return 0x1464;
  case 63:return 0x159A;
  case 64:return 0x16E3;
  case 65:return 0x183F;
  case 66:return 0x1981;
  case 67:return 0x1B38;
  case 68:return 0x1CD6;
  case 69:return 0x1E80;
  case 70:return 0x205E;
  case 71:return 0x224B; 
  case 72:return 0x2455; // C5$	554.37	9301	2455
  default:return 0x0;
    }
}

/**
 * Midi mapping.
 * C/DO is 60 __Octave3
 * C C# D D# E F F# G G# A A# B (71)
 * C= 60,72,84...120
 * C=48,36...0 
 * See http://phy.mtu.edu/~suits/NoteFreqCalcs.html
 * for a more dinamic formula...
 * fn = f0 * (a)^n 
 * a  is 1.059463094359... 
 */
int midi2FreqOLD(int midin){
  switch(midin){
  case 60:
    return NOTE_DO3;
  case 61:
    return NOTE_DOS3;
  case 62:
    return NOTE_RE3;
  case 63:
    return NOTE_RES3;
  case 64:
    return NOTE_MI3;
  case 65:
    return NOTE_FA3;
  case 66:
    return NOTE_FAS3;
  case 67:
    return NOTE_SOL3;
  case 68:
    return NOTE_SOLS3;
  case 69:
    return NOTE_LA3;
  case 70:
    return NOTE_LAS3;
  case 71:
    return NOTE_SI4;
  case 72:
    return NOTE_DO4;
  default:
    return 0;
  }
}

byte stopIfIVKeyPressed(void ){
  int keyval=analogRead(A0);
  if (keyval >=5 && keyval <=10){
    return 1;
  }else {
    return 0;
  }
}


/* See http://www.cprogramming.com/tutorial/function-pointers.html
 * 
 */
void playMusic(int voice, const int music[], void (*delayFunction)(long unsigned int),
               byte (*stop)(void)){
  int ctime=0;
  l("Music Start");
  for (int i=0; music[i] != -1;i+=2) {

    // +++ noTone(speakerOut);
    stopSound(voice);

    int midiNote=music[i+1];
    int du=music[i];

    playNote(voice,midi2Sid(midiNote));
    // ++ tone(speakerOut, midi2Freq(midiNote));
    
    int wm=du-ctime;
    // Adjust time
    wm *=1.5;
    if(wm <=0 ) { wm=1;}
    Serial.print(midiNote);
    Serial.print(" t:");
    Serial.print(wm);
    Serial.print(" -> ");
    delayFunction(wm);
    Serial.println("");
    ctime = du;
    if(stop() != 0){
      Serial.println(F("%% Requested STOP"));
      break;
    }
  }
  stopSound(voice);
  l("Music Ends.");
}

void playMusic(const int music[], void (*delayFunction)(long unsigned int)){
  playMusic(VOICE1,music,delayFunction,stopIfIVKeyPressed);
}
void playMusic(const int music[]){
  playMusic(music,delay);
}

void slowDown(long unsigned int x ){
  delay(x*1.2);
}


void speedUp(long unsigned int x ){
  int spDelay=x/1.5;
  if(spDelay >1000) {
    Serial.print(" * ");
    spDelay=1000;
  }
  
  Serial.print(spDelay);
  
  if( spDelay <=0) {
    delay(1);
  }else {
    delay(spDelay);
  }
}




void setup(){  
 pinMode(13, OUTPUT);

 Serial.begin(9600);
 l("Music Midi Player. SID PoweD");
 // Use jj-log.h
 l("CBM64 Like a cruel angel,");
 l("young boy, become the legend!");
 Serial.println();
 mySid.begin();
 //nilSysBegin();
 //tone(speakerOut,NOTE_DO4);
 //delay(1000);
 // SID Setup: VOICE1,2,3

 // Attack/decay; SUSTAIN/RELEASE
 mySid.set_register(VOICE1+5,9);
 mySid.set_register(VOICE1+6,9);

 mySid.set_register(VOICE2+5,0xA0);
 mySid.set_register(VOICE2+6,0xF0);

 mySid.set_register(VOICE3+5,0xA4);
 mySid.set_register(VOICE3+6,0xA0);

 //Do a demo with three different voice configuration

 mySid.set_register(24,15); // SET MAX VOLUME (unclear if needed)
 playMusic(VOICE1,midiTest1, &slowDown,&stopIfIVKeyPressed);
 playMusic(VOICE2,midiTest1, &slowDown,&stopIfIVKeyPressed);
 playMusic(VOICE3,midiTest1, &slowDown,&stopIfIVKeyPressed);
 //playMusic(axelTest2, &slowDown);
}


void loop() 
{
  int keyval=analogRead(A0);
  if(keyval !=0) {
    Serial.println(keyval);
  }
  if(keyval >=1022 ){
    playMusic(axelTest2, &speedUp);
  }else if (keyval >= 990 && keyval <= 1010 ){
    playMusic(midiTest1, &slowDown);
  }else if (keyval >=505 && keyval <=515){
  }else if (keyval >=5 && keyval <=10){
  }else{
    stopSound(VOICE1);
  }
  //
  //playMusic(midiTest1, &slowDown);
}

// Local variables:
// mode:c++
// mode:company
// End:
