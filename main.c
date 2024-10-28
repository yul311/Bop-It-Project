// Pin Definitions
const int encoderPinA = 4;   // CLK pin on KY-040
const int encoderPinB = 5;   // DT pin on KY-040
const int ledPin = 6;        // LED pin to indicate success
const int startGamePin = 1;  // Start button
const int greenLEDPin = 7;   // Green LED for success
const int redLEDPin = 8;     // Red LED for failure
const int audioPin = 9;      // Audio output
const int beamPin = 10;      // Beam sensor pin
const int hammerButton = 1;

int counter = 0;
bool success = false;
int timelimit = 1000; // 1-second time limit

void setup() {
  pinMode(startGamePin, INPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(audioPin, OUTPUT);
  pinMode(beamPin, INPUT);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);
}

void loop() {
  while (digitalRead(startGamePin) == LOW) {} // Wait for the game start button

  while (true) {
    int randomAction = random(0, 3);

    switch (randomAction) {
      case 0:
        playAudioCue("Saw");
        success = sawTask(timelimit);
        break;
      case 1:
        playAudioCue("Screwdriver");
        success = screwdriverTask(timelimit);
        break;
      case 2:
        playAudioCue("Hammer");
        success = hammerTask(timelimit);
        break;
    }

    if (success) {
      counter++;
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
    } else {
      digitalWrite(redLEDPin, HIGH);
      digitalWrite(greenLEDPin, LOW);
      break;
    }

    delay(500);

    if (counter == 99) {
      break;
    }
  }

  Serial.print("Final Score: ");
  Serial.println(counter);
  playAudioCue("Game Over");
  delay(1000); 
}

void playAudioCue(String action) {
  // Logic for audio cue based on action
}

bool sawTask(int timelimit) {
  unsigned long timer = millis();
  while (millis() - timer < timelimit) {
    if (digitalRead(beamPin) == LOW) { // Check if beam is broken
      return true;
    }
  }
  return false;
}

bool screwdriverTask(int timelimit) {
  unsigned long timer = millis();
  int encoderPosition = 0;
  int lastEncoderPinA = LOW;

  while (millis() - timer < timelimit) {
    int currentEncoderPinA = digitalRead(encoderPinA);

    // Detect rotation (rising edge of CLK)
    if (currentEncoderPinA != lastEncoderPinA && currentEncoderPinA == HIGH) {
      if (digitalRead(encoderPinB) != currentEncoderPinA) {
        encoderPosition++;  // Clockwise rotation
      } else {
        encoderPosition--;  // Counterclockwise rotation
      }

      if (encoderPosition >= 10) {
        return true;                 
      }
    }
    lastEncoderPinA = currentEncoderPinA;
  }
  return false;
}

bool hammerTask(int timelimit) {
  unsigned long timer = millis();
  while (millis() - timer < timelimit) {
    if (digitalRead(hammerButton) == HIGH) { // Check if beam is broken
      return true;
    }
  }
  return false;
}
