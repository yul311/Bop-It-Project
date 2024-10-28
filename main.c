#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(0, 1);  
DFRobotDFPlayerMini myDFPlayer;

// Pin Definitions
const int startGamePin = 6;
const int greenLEDPin = 7;
const int redLEDPin = 8;

// SAW
const int threshold = 100;
const int lightSensorPin = A0;

// Screwdriver
const int encoderPinA = 3;
const int encoderPinB = 4;

// Hammer
const int hammerButton = 9;

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
        playAudioCue(1, 4000);
        success = sawTask(timelimit);
        break;
      case 1:
        playAudioCue(2, 7000);
        success = screwdriverTask(timelimit);
        break;
      case 2:
        playAudioCue(3, 7000);
        success = hammerTask(timelimit);
        break;
    }

    if (success) {
      counter++;
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
      delay(500);  
    } else {
      digitalWrite(redLEDPin, HIGH);
      digitalWrite(greenLEDPin, LOW);
      delay(1000);  
      break;
    }

    
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
    success = false; 

    if (counter == 99) {
      break;
    }

    delay(500); 
  }

  Serial.print("Final Score: ");
  Serial.println(counter);
  
  delay(1000); 
}

void playAudioCue(int trackNumber, int delayTime)
{
  myDFPlayer.play(trackNumber);
  delay(delayTime);
}

bool sawTask(int timelimit)
{
  unsigned long timer = millis();
  while (millis() - timer < timelimit) {
    int sensorValue = analogRead(lightSensorPin);
    if (sensorValue < threshold) {
      return true;
    }
    delay(1);
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
