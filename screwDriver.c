// Pin Definitions
const int encoderPinA = 4;   // CLK pin on KY-040
const int encoderPinB = 5;   // DT pin on KY-040
const int ledPin = 6;        // LED pin to indicate success
int encoderPosition = 0;
int lastEncoderPinA = LOW;

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Make sure the LED is initially off
}

void loop() {
  int currentEncoderPinA = digitalRead(encoderPinA);

  // Detect rotation (rising edge of CLK)
  if (currentEncoderPinA != lastEncoderPinA && currentEncoderPinA == HIGH) {
    if (digitalRead(encoderPinB) != currentEncoderPinA) {
      encoderPosition++;  // Clockwise rotation
    } else {
      encoderPosition--;  // Counterclockwise rotation
    }
    
    // If encoder has turned 10 steps (threshold for testing)
    if (encoderPosition >= 10) {
      digitalWrite(ledPin, HIGH);  // Turn on LED to indicate success
      delay(2000);                 // Keep the LED on for 2 seconds
      digitalWrite(ledPin, LOW);   // Turn off LED
      encoderPosition = 0;         // Reset encoder position for next test
    }
  }

  lastEncoderPinA = currentEncoderPinA;
}
