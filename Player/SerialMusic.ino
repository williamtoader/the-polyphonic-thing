#include "./json.h"

#include <MusicWithoutDelay.h>
//const char note1[] PROGMEM = "C Scale:d=4:c,d,e,f,g,a,b,c1";   //plays c scale,  You can add more notes to get creative. //d=4 means that every note without a number in front of the letter is assumed to be a quarter note.
//const char note2[] PROGMEM = ":d=4:e,f,g,a,b,c1,d1,e1";   //plays c scale starting at e
const char note1[] PROGMEM = {":d=16,o=5,b=120:c,d,e"};   //plays c ,  You can add more notes to get creative.
const char note2[] PROGMEM = {":d=16,o=5,b=120:e,f,g"};  //plays e
const char note3[] PROGMEM = {":d=16,o=5,b=120:f,g,a"};  //plays f
const char note4[] PROGMEM = {":d=16,o=5,b=120:g,a,b"};  //plays g
MusicWithoutDelay pianoKey(note1);
MusicWithoutDelay pianoKey2(note2);
MusicWithoutDelay pianoKey3(note3);
MusicWithoutDelay pianoKey4(note4);

StaticJsonDocument<200> doc;


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  // put your setup code here, to run once:
  pianoKey.begin(CHB, TRIANGLE, ENVELOPE1, 0);
  pianoKey2.begin(CHB, TRIANGLE, ENVELOPE1, 0);
  pianoKey3.begin(CHB, TRIANGLE, ENVELOPE1, 0);
  pianoKey4.begin(CHB, TRIANGLE, ENVELOPE1, 0);

  //pianoKey.pause(true); pianoKey2.pause(true); pianoKey3.pause(true); 
  //pianoKey4.pause(true);
  pianoKey.play(); 
  pianoKey2.play(); 
  pianoKey3.play(); 
  pianoKey4.play();
  deserializeJson(doc, "{\"s1\":0,\"s2\":0,\"s3\":0,\"s4\":0}");
}

String code = "";
char inChar = '\n';
char inData[256] = "";
int index = 0;
bool append_c=true;
void loop() {
  // put your main code here, to run repeatedly:
  pianoKey.update();
  pianoKey2.update();
  pianoKey3.update();
  pianoKey4.update();

  
  //code = Serial.readStringUntil('\n');

    while (Serial.available() > 0) // Don't read unless
    // there you know there is data
    {
       if(index < 255) // One less than the size of the array
       {
           inChar = Serial.read(); // Read a character
           if(inChar == '\n') {
             append_c = false;
           }
           else {
            if(append_c){code.concat(inChar);}
            else {code = "";append_c=true;code.concat(inChar);}
           }
       }
        pianoKey.update();
        pianoKey2.update();
        pianoKey3.update();
        pianoKey4.update();
       
    }
  Serial.println(code);
  
  if(!append_c && code.length() > 0) deserializeJson(doc, code);
  else deserializeJson(doc, "{\"s1\":0,\"s2\":0,\"s3\":0,\"s4\":0}");
  
  buttonToPiano(1,(int)doc["s1"] > 0, pianoKey,  (int)doc["s1"]);
  buttonToPiano(2,(int)doc["s2"] > 0, pianoKey2, (int)doc["s2"]);
  buttonToPiano(3,(int)doc["s3"] > 0, pianoKey3, (int)doc["s3"]);
  buttonToPiano(4,(int)doc["s4"] > 0, pianoKey4, (int)doc["s4"]);
  
  //{"s1":30,"s2":-1,"s3":-1,"s4":-1}
}

void buttonToPiano(int a, bool b, MusicWithoutDelay &m, int dist) { 
  
  if(a == 4) {
    if(b) {m.mute(false);m.setFrequency(
      (double)440 
      * pow(2,(double)((int)dist / 40)/12.0)
      );
      }
    else m.mute(true);
  }
  else m.mute(!b);
  //else m.pause(true);
}
