#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <DHT.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Students"
#define WIFI_PASSWORD "JT@wbm61s"

// Insert Firebase project API Key
#define API_KEY "AIzaSyC1gujNfFh6QtjYIwKVf9YbC9OzOS0BzHk"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "cck01092005@gmail.com"
#define USER_PASSWORD "Kanha#2005"

// Insert RTDB URL
#define DATABASE_URL "https://esp-32-iot-f1178-default-rtdb.firebaseio.com"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String tempPath;
String humPath;
String objectDetectPath;

// DHT11 sensor configuration
#define DHTPIN D4  // Define the GPIO pin connected to the DHT11 data pin
#define DHTTYPE DHT11

// IR sensor configuration
#define IR_SENSOR_PIN D2  // Define the GPIO pin connected to the IR sensor

DHT dht(DHTPIN, DHTTYPE);

float temperature;
float humidity;

// Timer variables (send new readings every 10 seconds)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 10000; // 10 seconds

// Initialize DHT11
void initDHT() {
  dht.begin();
  Serial.println("DHT11 sensor initialized.");
}

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());
}

// Write float values to the database
void sendFloat(String path, float value) {
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)) {
    Serial.print("Writing value: ");
    Serial.print(value);
    Serial.print(" to path: ");
    Serial.println(path);
    Serial.println("Write successful!");
  } else {
    Serial.print("Write failed: ");
    Serial.println(fbdo.errorReason());
  }
}

// Write boolean values to the database
void sendBool(String path, bool value) {
  if (Firebase.RTDB.setBool(&fbdo, path.c_str(), value)) {
    Serial.print("Writing value: ");
    Serial.print(value);
    Serial.print(" to path: ");
    Serial.println(path);
    Serial.println("Write successful!");
  } else {
    Serial.print("Write failed: ");
    Serial.println(fbdo.errorReason());
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT11 sensor and WiFi
  initDHT();
  initWiFi();

  // Set up IR sensor as input
  pinMode(IR_SENSOR_PIN, INPUT);

  // Assign the API key (required)
  config.api_key = API_KEY;

  // Assign the user sign-in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long-running token generation task
  config.token_status_callback = tokenStatusCallback; // See addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase auth and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database paths for sensor readings
  tempPath = databasePath + "/temperature"; // --> UsersData/<user_uid>/temperature
  humPath = databasePath + "/humidity";     // --> UsersData/<user_uid>/humidity
  objectDetectPath = databasePath + "/objectDetected"; // --> UsersData/<user_uid>/objectDetected
}

void loop() {
  // Send new readings to database every 10 seconds
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Get latest sensor readings
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Display readings on Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Send readings to database
    sendFloat(tempPath, temperature);
    sendFloat(humPath, humidity);

    // Read IR sensor value
    bool objectDetected = digitalRead(IR_SENSOR_PIN) == LOW; // Adjust based on your IR sensor's logic
    Serial.print("Object Detected: ");
    Serial.println(objectDetected);

    // Send the object detection status to Firebase
    sendBool(objectDetectPath, objectDetected);
  }
}


