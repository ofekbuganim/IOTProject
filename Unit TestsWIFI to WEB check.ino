#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

// Wi-Fi credentials
const char* ssid = "AndroidAP55C0";
const char* password = "nhgs2148";

// ThingSpeak API Key
const char* thingSpeakApiKey = "DGMC6S9EPLRSOFZU";

// DHT11 Sensor
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Wind Sensor
const int analogPin = 34;  // A0 -> GPIO34

// BMP280
#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_ADDRESS 0x76
Adafruit_BMP280 bmp;

// UV Sensor
#define UV_SENSOR_PIN 34  // Also using A0 (for demo purposes)

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!bmp.begin(I2C_ADDRESS)) {
    Serial.println("BMP280 not detected. Check wiring.");
  } else {
    Serial.println("BMP280 initialized.");
  }

  connectToWiFi();
  analogReadResolution(12); // 12-bit ADC (0–4095)
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  int rawWind = analogRead(analogPin);
  float voltageWind = (rawWind / 4095.0) * 3.3;
  float windSpeed = voltageWind * 3.0 * 3.6;  // Rough estimate

  float pressure = bmp.readPressure() / 100.0F;
  float altitude = bmp.readAltitude(1013.25);

  int uvRaw = analogRead(UV_SENSOR_PIN);
  float uvVoltage = (uvRaw / 4095.0) * 3.3;
  float uvIndex = (uvVoltage - 0.99) * (15.0 / (2.8 - 0.99));
  if (uvIndex < 0) uvIndex = 0;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + String(thingSpeakApiKey) +
                 "&field1=" + String(temp) +
                 "&field2=" + String(hum) +
                 "&field3=" + String(windSpeed) +
                 "&field4=" + String(pressure) +
                 "&field5=" + String(altitude) +
                 "&field6=" + String(uvIndex);

    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("Data sent! Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending data. Code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(15000); // ThingSpeak allows updates every 15 seconds
}
