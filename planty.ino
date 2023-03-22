//****************************************************
//  Imports
//****************************************************

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

#include <DHT.h>

// Time Updating
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "UUID.h"

//****************************************************
//  Constants
//****************************************************

// Temp sensor constants
#define DHT_SENSOR_PIN  4 // ESP32 pin GIOP21 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22


// UV light sensor constants
#define GUVA_SENSOR_PIN 11

// Moisture sensor constants
#define SOIL_MOISTURE_SENSOR_ANALOG_PIN 26
#define SOIL_MOISTURE_SENSOR_DIGITAL_PIN 27

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// Wifi credentials
#define WIFI_SSID "MyAltice e3219d"
#define WIFI_PASSWORD "24-chestnut-8872"

// Firebase firestore constants
#define API_KEY "AIzaSyD3m_2WZNJXZ_3So28xISJ1vRXGrvfINv0"
#define FIREBASE_PROJECT_ID "planty-ca2a1"
#define USER_EMAIL "daniel@gmail.com"
#define USER_PASSWORD "always55"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;

// Variables to save date and time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;

UUID sessionUUID;

//****************************************************
//  Setup
//****************************************************

void setup()
{
  Serial.begin(115200);

  pinMode(SOIL_MOISTURE_SENSOR_ANALOG_PIN, INPUT);    // sets the digital pin 13 as output

  setupWifi();
  setupFirebase(); 
  setupTimeClient(); 
  setupTempSensor();
}

//****************************************************
//  Loop
//****************************************************

void loop()
{  
  long interval = 600000; // 10 minutes
  long hourInterval = 3600000; // 1 hour

  if (Firebase.ready() && (millis() - dataMillis > interval || dataMillis == 0))
  {
    dataMillis = millis();
    createDataPoint(); 
  }
}