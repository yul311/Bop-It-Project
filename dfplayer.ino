#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(0, 1);
DFRobotDFPlayerMini myDFPlayer;

// Running into issues with playing an audio file after the first successful audio file

void setup () {
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(2, OUTPUT);

  mySerial.begin(9600);
  myDFPlayer.begin(mySerial); //set Serial for DFPlayer-mini mp3 module 
  myDFPlayer.volume(20);
}
 
void loop () {        
  if(digitalRead(8) == HIGH){  
    digitalWrite(2, HIGH);   
    myDFPlayer.play(1); //play 0001.mp3
    delay(4000); //4 sec, time delay to allow 0001.mp3 to finish playing
    digitalWrite(2, LOW);
  }
  if(digitalRead(9) == HIGH){
    digitalWrite(2, HIGH);
    myDFPlayer.play(2); //play 0002.mp3
    delay (7000); //7 sec, time delay to allow 0002.mp3 to finish playing
    digitalWrite(2, LOW);
  }
  
}