// Pin Definitions
const int lightSensorPin = A0;  // Light sensor connected to analog pin
const int ledPin = 6;           // LED pin to indicate saw action detected
int threshold = 500;            // Adjust this based on your sensor setup

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Make sure the LED is initially off
  
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(lightSensorPin);
  
  Serial.print("Light Sensor Value: ");
  Serial.println(sensorValue);
  
  // If the sensor value is below the threshold (saw is blocking the light)
  if (sensorValue < threshold) {
    digitalWrite(ledPin, HIGH);  // Turn on LED to indicate action detected
    delay(2000);                 // Keep the LED on for 2 seconds
    digitalWrite(ledPin, LOW);   // Turn off LED
  }
  
  delay(100); // Small delay between readings
}
