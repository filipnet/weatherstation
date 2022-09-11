#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#define MQTT_MAX_PACKET_SIZE 256
#include <SHT1x-ESP.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <SPI.h>
#include "credentials.h"
#include "config.h"

const char *hostname = WIFI_HOSTNAME;
const char *ssid = WIFI_SSID;
const char *password =  WIFI_PASSWORD;
const char *mqttServer = MQTT_SERVER;
const int mqttPort = MQTT_PORT;
const char *mqttUser = MQTT_USERNAME;
const char *mqttPassword = MQTT_PASSWORD;
const char *mqttID = MQTT_ID;
const int measurementInterval = MEASUREMENT_INTERVAL;
const int reedPin = REED_PIN;

// Define global variables 
unsigned long heartbeat_previousMillis = 0;
const long heartbeat_interval = HEARTBEAT_INTERVALL;
float temperature_local;
float humidity_local;

// Define multiplex ports
const int selectPins[3] = {D5, D6, D7}; // S0~2, S1~3, S2~4
const int zOutput = 5;
const int zInput = A0; // Connect common (Z) to A0 (analog input)

// Assign the SPI bus to pins
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
Adafruit_BME280 bme;         // I2C (2 wire mode)
#define BME280_I2C_ADDR 0x76 // Set I2C-Address to BME280 Sensor
// Set local pressure values
#define SEALEVELPRESSURE_HPA (1013.25)
#define LOCALPRESSURE_HPA (1013.25) // 154m Germany, Rhineland-Palatinate, Lörzweiler

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

WiFiClientSecure espClient;
PubSubClient client(espClient);
 
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.printf("Sketch size: %u\n", ESP.getSketchSize());
  Serial.printf("Free size: %u\n", ESP.getFreeSketchSpace());
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(dataPin, INPUT);
  pinMode(clockPin, INPUT);
  pinMode(reedPin, INPUT_PULLUP);
  espClient.setInsecure();
  reconnect();
  initializeMux();
  testSensors();
}

void reconnect() {
  while (!client.connected()) {
    WiFi.mode(WIFI_STA);
    WiFi.hostname(hostname);
    delay(100);
    Serial.println();
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(50);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to WiFi network");
    Serial.print("  SSID: ");
    Serial.print(ssid);
    Serial.print(" / Channel: ");
    Serial.print(WiFi.channel());
    Serial.print(" / RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("  IP Address: ");
    Serial.print(WiFi.localIP());
    Serial.print(" / Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("  Gateway: ");
    Serial.print(WiFi.gatewayIP());
    Serial.print(" / DNS: ");
    Serial.print(WiFi.dnsIP());
    Serial.print(", ");
    Serial.println(WiFi.dnsIP(1));
    Serial.println("");

    // https://pubsubclient.knolleary.net/api.html
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    Serial.print("Connecting to MQTT broker: ");
    Serial.println(mqttServer);

    while (!client.connected()) {
      delay(50);
      Serial.print(".");      
      if (client.connect(mqttID, mqttUser, mqttPassword)) {
        Serial.println();
        Serial.println("Connected to MQTT broker");
        Serial.print("  MQTT Server: ");
        Serial.println(mqttServer);
        Serial.print("  MQTT Port: ");
        Serial.println(mqttPort);
        Serial.print("  MQTT Username: ");
        Serial.println(mqttUser);
        Serial.print("  MQTT Identifier: ");
        Serial.println(mqttID);
        Serial.println("");
        digitalWrite(LED_BUILTIN, HIGH); 
      } else {
        Serial.print("Connection to MQTT broker failed with state: ");
        Serial.println(client.state());
        char puffer[100];
        espClient.getLastSSLError(puffer,sizeof(puffer));
        Serial.print("TLS connection failed with state: ");
        Serial.println(puffer);
        Serial.println("");
        delay(4000);
       }
    }
  }
}

// Function to receive MQTT messages
void mqttloop() {
  if (!client.loop())
    client.connect("ESP8266Client");
}

// Function to send MQTT messages
void mqttsend(const char *_topic, const char *_data) {
  client.publish(_topic, _data);
}

// Pointer to a message callback function called when a message arrives for a subscription created by this client.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message topic: ");
  Serial.print(topic);
  Serial.print(" | Message Payload: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
}

void loop() {
  client.loop();
  mqttloop();
  reconnect();
  heartbeat();
  readsensor_raingauge();

  now = millis();
  if (now - lastMeasure > measurementInterval) {
    lastMeasure = now;
    readsensor_ldr();
    readsensor_bme280();
    void readsensor_sht10();
    Serial.println("");
  }
}

void heartbeat() {
  unsigned long heartbeat_currentMillis = millis();
  if (heartbeat_currentMillis - heartbeat_previousMillis >= heartbeat_interval) {
    heartbeat_previousMillis = heartbeat_currentMillis;
    Serial.println("Send heartbeat signal to MQTT broker");
    Serial.println("");
    client.publish("home/outdoor/weather/heartbeat", "on");
  }
}

// initializeMux (in setup) set up the selected multiplex pins as outputs, Set up Z as an input
void initializeMux() {
  for (int i = 0; i < 3; i++) {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }
  pinMode(zInput, INPUT);
}

// The selectMuxPin function sets the S0, S1, and S2 pins accordingly, given a pin from 0-7
void selectMuxPin(byte pin) {
  for (int i = 0; i < 3; i++) {
    if (pin & (1 << i)) {
      digitalWrite(selectPins[i], HIGH);
    } else { 
      digitalWrite(selectPins[i], LOW);
    }
  }
}

// Test sensor
void testSensors() {
  Serial.print("Testing BME280 pressure module: ");
  if (!bme.begin(BME280_I2C_ADDR)) {
    Serial.println(F("Failed to read from BMP280 sensor, check wiring"));
  } else {
    Serial.println("OK");
  }
  Serial.println("");
}

void readsensor_ldr() {
  selectMuxPin(1);
  int b = (analogRead(A0));
  Serial.print("Brightness: ");
  Serial.print(b);
  Serial.println(" RAW");
  static char brightnessTemp[7];
  dtostrf(b, 6, 2, brightnessTemp);
  Serial.print("  MQTT publish home/outdoor/weather/brightness/raw: ");
  Serial.println(brightnessTemp);
  client.publish("home/outdoor/weather/brightness/raw", brightnessTemp, true);
  delay(100);
}

int PulseState;                       // the current reading from the input pin
int lastPulseState = HIGH;            // the previous reading from the input pin
unsigned long lastDebounceTime = 0;   // the last time the output pin was toggled
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers

void readsensor_raingauge() {
  int pulse = digitalRead(REED_PIN);
  if (pulse != lastPulseState) {
    lastDebounceTime = millis();
    if (PulseState == HIGH) {
        Serial.println("Rain gauge pulse");
        Serial.print("  MQTT publish home/outdoor/weather/raingauge/pulse: ");
        Serial.println("pulse");
        client.publish("home/outdoor/weather/raingauge/pulse", "pulse", false);
        delay(100);
      }
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (pulse != PulseState) {
      PulseState = pulse;
     }
  }
  lastPulseState = pulse;
}

void readsensor_bme280() {
  float t = bme.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
  static char temperatureTemp[7];
  dtostrf(t, 6, 2, temperatureTemp);
  Serial.print("  MQTT publish home/outdoor/weather/temperature: ");
  Serial.println(temperatureTemp);
  client.publish("home/outdoor/weather/temperature", temperatureTemp, true);
  delay(100);

  float h = bme.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
  static char humidityTemp[7];
  dtostrf(h, 6, 2, humidityTemp);
  Serial.print("  MQTT publish home/outdoor/weather/humidity: ");
  Serial.println(humidityTemp);
  client.publish("home/outdoor/weather/humidity", humidityTemp, true);
  delay(100);

  float p = (bme.readPressure() / 100.0F);
  Serial.print("Pressure: ");
  Serial.print(p);
  Serial.println(" hPa");
  static char pressureTemp[7];
  dtostrf(p, 6, 2, pressureTemp);
  Serial.print("  MQTT publish home/outdoor/weather/pressure: ");
  Serial.println(pressureTemp);
  client.publish("home/outdoor/weather/pressure", pressureTemp, true); // Pressure (hPa)
  delay(100);

  float a = bme.readAltitude(LOCALPRESSURE_HPA);
  Serial.print("Approx. Altitude: ");
  Serial.print(a);
  Serial.println(" m");
  static char altitudeTemp[7];
  dtostrf(a, 6, 2, altitudeTemp);
  Serial.print("  MQTT publish home/outdoor/weather/altitude: ");
  Serial.println(altitudeTemp);
  client.publish("home/outdoor/weather/altitude", altitudeTemp, true); // Approx altitude (m)
  delay(100);
}

void readsensor_sht10() {
  temperature_local = sht1x.readTemperatureC();
  Serial.print("Temperature: ");
  Serial.print(temperature_local, DEC);
  Serial.println(" *C");
  static char temperature_local_char[7];
  dtostrf(temperature_local, 1, 2, temperature_local_char);
  Serial.print("  MQTT publish home/outdoor/weather/temperature: ");
  Serial.println(temperature_local_char);
  client.publish("home/outdoor/weather/temperature", temperature_local_char, true);
  delay(100);

  humidity_local = sht1x.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity_local);
  Serial.println(" %");
  static char humidity_local_char[7];
  dtostrf(humidity_local, 1, 2, humidity_local_char);
  Serial.print("  MQTT publish home/outdoor/weather/humidity: ");
  Serial.println(humidity_local_char);
  client.publish("home/outdoor/weather/humidity", humidity_local_char, true);
  delay(100);
}
