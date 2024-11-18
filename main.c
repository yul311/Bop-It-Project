#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(10, 9);  
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

// Analog pin for random seed noise
const int randomSeedPin = A0;

int counter = 0;
bool success = true;
int timelimit = 5000;


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

 // Initialize serial communication for the DFPlayer
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini initialization failed!");
    while (true);
  }
  else{

  myDFPlayer.begin(mySerial);  // Set output to SD card
  myDFPlayer.volume(30);
  }


  // Initialize random seed based on noise from analog pin
  randomSeed(analogRead(randomSeedPin));
}

void loop()
{

  while(success == true){

  counter = 0;
  int timelimit = 3500;

  while (true) {
    int randomAction = random(0, 3);  // Choose a random task

    switch (randomAction) {
      case 0:
        playAudioCue(1);  // Audio cue for saw task
        success = sawTask(timelimit);
        break;
      case 1:
        playAudioCue(2);  // Audio cue for screwdriver task
        success = screwdriverTask(timelimit);
        break;
      case 2:
        playAudioCue(3);  // Audio cue for hammer task
        success = hammerTask(timelimit);
        break;
    }

    if (success) {
      counter++;
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
      timelimit *=.95;
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

  //Play final audio cue based on the score
  playAudioCue(5);
  delay(1200);
  playAudioCue(counter+6);
  delay(1000); 
}
}

void playAudioCue(int trackNumber)
{
  myDFPlayer.play(trackNumber);
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
      if (encoderPosition >= 4) {
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
