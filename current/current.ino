// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
// }
// void loop() {
//   // put your main code here, to run repeatedly:
//   int adc = analogRead(D6);
//   float voltage = adc*5/1023.0;
//   float current = (voltage-2.5)/0.185;
//   Serial.print("Current : ");
//   Serial.println(current);
//   delay(1000);
// }

// const int currentSensorPin = D6;  // Analog pin connected to the ACS712 sensor
// const float sensitivity = 0.100;  // Sensitivity of ACS712 in V/A (e.g., 0.100 for 20A module)
// const float Vref = 3.3;           // Reference voltage of NodeMCU (3.3V)
// const int ADCresolution = 1024;   // ADC resolution for ESP8266 (0 to 1023)
// const float zeroCurrentVoltage = Vref / 2;  // Output voltage at 0A (midpoint)

// void setup() {
//   Serial.begin(115200);  // Initialize serial communication
//   delay(1000);
//   Serial.println("Current Measurement with ACS712");
// }

// void loop() {
//   // Read the analog value from the sensor
//   int sensorValue = analogRead(currentSensorPin);

//   // Convert the analog reading to voltage
//   float voltage = (sensorValue / (float)ADCresolution) * Vref;

//   // Calculate the current using the sensor's sensitivity and zero-current voltage
//   float current = (voltage - zeroCurrentVoltage) / sensitivity;

//   // Print the measured current to the Serial Monitor
//   Serial.print("Measured Current: ");
//   Serial.print(current, 3);  // Display current with 3 decimal places
//   Serial.println(" A");

//   delay(1000);  // Delay between readings
// }






#include "ACS712.h"



/*

  This example shows how to measure DC current

*/



// We have 30 amps version sensor connected to A1 pin of arduino

// Replace with your version if necessary

ACS712 sensor(ACS712_30A, D6);



void setup() {

  Serial.begin(9600);



  // This method calibrates zero point of sensor,

  // It is not necessary, but may positively affect the accuracy

  // Ensure that no current flows through the sensor at this moment

  sensor.calibrate();

}



void loop() {

  // Get current from sensor

  float  I = sensor.getCurrentDC();

  Serial.println(String("I = ") + I+ " A");

  

  // Wait one second before the new cycle

  delay(1000);

}
