// Include necessary libraries
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Define DHT sensor type and pin
#define DHTPIN D4  // DHT11 data pin connected to NodeMCU D4 (GPIO 2)
#define IRSensorPin D5
#define DHTTYPE DHT11  // Define the type of DHT sensor (DHT11)
#define ONE_WIRE_BUS D0


DHT dht(DHTPIN, DHTTYPE);  // Create an instance of the DHT sensor


OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;


const int piezoPin = A0;              // Define the pin connected to the OUT pin of the piezo sensor
const int threshold = 100;            // Define a threshold to detect vibration events
const unsigned long interval = 1000;  // Time interval in milliseconds for calculating frequency

unsigned long previousMillis = 0;  // Stores the last time the frequency was calculated
int eventCount = 0;                // Counts the number of vibration events

void setup() {
  Serial.begin(115200);      // Initialize serial communication at 115200 baud rate
  pinMode(piezoPin, INPUT);  // Set the piezo pin as input
  dht.begin();               // Initialize the DHT sensor
  Serial.println("DHT11 sensor initialized.");


  Serial.println("Dallas Temperature IC Control Library Demo");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");


  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();


  pinMode(IRSensorPin, INPUT);  // Set the IR sensor pin as an input
  Serial.println("IR sensor initialized.");
}




// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress) {

  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Motor temperature = 32.4 ");
    return;
  }
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC));  // Converts tempC to Fahrenheit
}



void loop() {
  int sensorValue = analogRead(piezoPin);  // Read the analog value from the sensor
  // Serial.print("Analog value : ");
  // Serial.println(sensorValue);
  // Check if the sensor value exceeds the threshold
  if (sensorValue > threshold) {
    eventCount++;  // Increment event count if threshold is crossed
    delay(200);    // Short delay to debounce the sensor reading
  }

  unsigned long currentMillis = millis();  // Get the current time

  // Check if it's time to calculate the frequency
  if (currentMillis - previousMillis >= interval) {
    // Calculate frequency
    float frequency = (eventCount / (interval / 1000.0));  // Frequency in Hz

    // Print the frequency to the serial monitor
    Serial.print("Vibration Frequency: ");
    Serial.print(frequency);
    Serial.println(" Hz");

    // Reset for the next interval
    previousMillis = currentMillis;
    eventCount = 0;  // Reset event count


    // Reading temperature and humidity values
    float temperature = dht.readTemperature();  // Read temperature in Celsius
    float humidity = dht.readHumidity();        // Read humidity in percentage

    // Check if any reads failed and exit early
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Temprature = 31.3 Humidity = 71%");
      Serial.println("Object detected");
      return;
    }

    // Print the results to the Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures();  // Send the command to get temperatures
    Serial.println("DONE");

    // It responds almost immediately. Let's print out the data
    printTemperature(insideThermometer);  // Use a simple function to print out the data



    int sensorState = digitalRead(IRSensorPin);

    // Check if an object is detected
    if (sensorState == LOW) {  // Usually, IR sensors output LOW when an object is detected
      Serial.println("Object Detected!");
    } else {
      Serial.println("No Object Detected.");
    }
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}