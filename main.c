#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(0, 1);
DFRobotDFPlayerMini myDFPlayer;

// Pin Definitions
const int startGamePin = 6;
const int greenLEDPin = 8;
const int redLEDPin = 7;

// SAW
const int threshold = 100;
const int lightSensorPin = A0;
// 

//ScrewDriver
const int encoderPinA = 3;
const int encoderPinB= 4;

//hammer
const int hammerButton = 9;
//

//Sound

//

int counter = 0;
bool success = false;
int timelimit = 2000; // 1-second time limit

void setup()
{
  pinMode(startGamePin, INPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  digitalWrite(greenLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);

  mySerial.begin(9600);
  myDFPlayer.begin(mySerial); // set Serial for DFPlayer-mini mp3 module
  myDFPlayer.volume(20);
}

void loop()
{
  while (digitalRead(startGamePin) == LOW)
  {
  } // Wait for the game start button

  while (true)
  {

    int randomAction = random(0, 3);

    switch (randomAction)
    {
    case 0:
      playAudioCue(1, 4);
      success = sawTask(timelimit);
      break;
    case 1:
      playAudioCue(2, 7);
      success = screwdriverTask(timelimit);
      break;
    case 2:
      playAudioCue(3, 7);
      success = hammerTask(timelimit);
      break;
    }

    if (success)
    {
      counter++;
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
    }
    else
    {
      digitalWrite(redLEDPin, HIGH);
      digitalWrite(greenLEDPin, LOW);
      break;
    }

    delay(500);

    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, LOW);

    if (counter == 99)
    {
      break;
    }
  }

  Serial.print("Final Score: ");
  Serial.println(counter);
  // playAudioCue(3, 7); //going to be game over.
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
  while (millis() - timer < timelimit)
  {
    int sensorValue = analogRead(lightSensorPin);

    // If the sensor value is below the threshold (saw is blocking the light)
    if (sensorValue < threshold)
    {
      return true;
    }
    delay(1); // Small delay between readings
  }
  return false;
}

bool screwdriverTask(int timelimit)
{
  unsigned long timer = millis();
  int encoderPosition = 0;
  int lastEncoderPinA = LOW;

  while (millis() - timer < timelimit)
  {
    int currentEncoderPinA = digitalRead(encoderPinA);

    // Detect rotation (rising edge of CLK)
    if (currentEncoderPinA != lastEncoderPinA && currentEncoderPinA == HIGH)
    {
      if (digitalRead(encoderPinB) != currentEncoderPinA)
      {
        encoderPosition++; // Clockwise rotation
      }
      else
      {
        encoderPosition--; // Counterclockwise rotation
      }

      if (encoderPosition >= 10)
      {
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
  while (millis() - timer < timelimit)
  {
    if (digitalRead(hammerButton) == HIGH)
    { // Check if beam is broken
      return true;
    }
  }
  return false;
}
