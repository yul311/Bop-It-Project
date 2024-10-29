#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(9, 10);  
DFRobotDFPlayerMini myDFPlayer;

// Pin Definitions
const int startGamePin = 6;
const int greenLEDPin = 7;
const int redLEDPin = 8;

// SAW
const int lightSensorPin = 5;

// Screwdriver
const int encoderPinA = 3;
const int encoderPinB = 4;

// Hammer
const int hammerButton = 2;

int counter = 0;
bool success = false;
int timelimit = 2000;

void setup()
{
  pinMode(startGamePin, INPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(hammerButton, INPUT);

  digitalWrite(greenLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);

  mySerial.begin(9600);
   
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini initialization failed!");
    while (true);
  }
  else{

  myDFPlayer.begin(mySerial); 
  myDFPlayer.volume(20);
  }
}

void loop()
{
  while (digitalRead(startGamePin) == LOW) {}

  counter = 0;

  while (true) {
    int randomAction = random(0, 3);

    switch (randomAction) {
      case 0:
        playAudioCue(1);
        success = sawTask(timelimit);
        break;
      case 1:
        playAudioCue(2);
        success = screwdriverTask(timelimit);
        break;
      case 2:
        playAudioCue(3);
        success = hammerTask(timelimit);
        break;
    }

    if (success) {
      counter++;
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
      delay(1000);  
      digitalWrite(greenLEDPin, LOW);
    } else {
      digitalWrite(redLEDPin, HIGH);
      digitalWrite(greenLEDPin, LOW);
      delay(1000); 
      digitalWrite(redLEDPin, LOW); 
      break;
    }

    if (counter == 99) {
      break;
    }

    delay(500); 
  }

  Serial.print("Final Score: ");
  Serial.println(counter);

  playAudioCue(4);
  
  delay(1000); 
}

void playAudioCue(int trackNumber)
{
  myDFPlayer.play(trackNumber);

  // Wait until the track is finished
  delay(3000);
}

bool sawTask(int timelimit)
{
  unsigned long timer = millis();
  while (millis() - timer < timelimit) {
    if (digitalRead(lightSensorPin) == LOW) {
      return true;
    }
  }
  return false;
}

bool screwdriverTask(int timelimit)
{
  unsigned long timer = millis();
  int encoderPosition = 0;
  int lastEncoderPinA = LOW;

  while (millis() - timer < timelimit) {
    int currentEncoderPinA = digitalRead(encoderPinA);
    if (currentEncoderPinA != lastEncoderPinA && currentEncoderPinA == HIGH) {
      if (digitalRead(encoderPinB) != currentEncoderPinA) {
        encoderPosition++;
      } else {
        encoderPosition--;
      }
      if (encoderPosition >= 10) {
        return true;
      }
    }
    lastEncoderPinA = currentEncoderPinA;
  }
  return false;
}

bool hammerTask(int timelimit)
{
  unsigned long timer = millis();
  while (millis() - timer < timelimit) {
    if (digitalRead(hammerButton) == HIGH) {
      return true;
    }
  }
  return false;
}
