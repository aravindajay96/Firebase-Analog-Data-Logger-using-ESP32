/*
  -------------------------------------------------------------------
  Project: Firebase Analog Data Logger using ESP32
  Author:  Aravinda Jayakody
  Description:
    This project reads analog sensor data from ESP32 and uploads it
    to Firebase Realtime Database every 5 seconds using HTTP requests.
    The data is sent in JSON format with a timestamp and sensor value.
  -------------------------------------------------------------------
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define WIFI_SSID "**Your_WIFI_SSID**"
#define WIFI_PASS "**Your_WIFI_Password**"
#define FIREBASE_HOST "**Your_Project_ID.firebaseio.com**"  // Without "/"
#define FIREBASE_SECRET "**Firebase_Database_Secret**"


#define ANALOG_PIN 34  // Any analog pin

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnected to WiFi!");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        float analogValue = analogRead(ANALOG_PIN);
        sendDataToFirebase(analogValue);
    } else {
        Serial.println("WiFi not connected!");
    }
    delay(5000);  // Send data every 5 seconds
}

void sendDataToFirebase(float value) {
    HTTPClient http;
    String firebaseURL = String(FIREBASE_HOST) + "/TestData.json?auth=" + FIREBASE_SECRET;

    // Create JSON object
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["timestamp"] = millis();
    jsonDoc["value"] = value;

    String jsonStr;
    serializeJson(jsonDoc, jsonStr);

    // Send POST request
    http.begin(firebaseURL);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.PUT(jsonStr);  // Use PUT to overwrite data, POST for appending

    Serial.print("Firebase response: ");
    Serial.println(httpResponseCode);
    
    http.end();
}
