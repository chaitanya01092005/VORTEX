const int IR_PIN = D3;  // Define the pin connected to the IR sensor
int objectDetected = 0;  // Variable to store the sensor state

void setup() {
  pinMode(IR_PIN, INPUT);  // Set the IR pin as an input
  Serial.begin(9600);      // Initialize serial communication for debugging
}

void loop() {
  // Read the state of the IR sensor
  objectDetected = digitalRead(IR_PIN);

  // Check if an object is detected (LOW or HIGH depends on your sensor)
  if (objectDetected == LOW) {  // Adjust to HIGH if your sensor outputs HIGH when detecting
    Serial.println("Object Detected!");
  } else {
    Serial.println("No Object Detected");
  }

  delay(500);  // Delay to avoid too many messages; adjust as needed
}
