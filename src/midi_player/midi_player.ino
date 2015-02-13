
/** The midi player tester
  */
#include <pitches_it.h>
//#include <NilRTOS.h>
#include <jj-log.h>
// #define DEBUG yeppa


//const int speakerOut=A5;
const int speakerOut=8;


const int DelayTime=44; // 44 is good

const float  durationBase=800;
const float D1_3= 0.33;

const float HALF= 0.5;
const float Q   = 0.25;


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

/* 
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


/**
 * Midi mapping.
 * C/DO is 60 __Octave3
 * C C# D D# E F F# G G# A A# B (71)
 * C= 60,72,84...120
 * C=48,36...0 
 */
void playMusic(const float music[]){
}
    
void setup(){  
 pinMode(13, OUTPUT);

 Serial.begin(9600);
 l("Music Midi Player");
 // Use jj-log.h
 l("Like a cruel angel,");
 l("young boy, become the legend!");
 Serial.println();
 //nilSysBegin();
 tone(speakerOut,NOTE_DO4);
 delay(1000);
 noTone(speakerOut);
}

void loop() 
{
  //playMusic(scalaMusicale);
}
// Local variables:
// mode:c++
// mode:company
// End:
