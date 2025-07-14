/*
  ESP32 ESP-NOW Sender
  Sends temperature, humidity, and reading ID to a receiver via ESP-NOW
*/

#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include <math.h>

//UV
#define UV_SENSOR_PIN 35  // D connected to A
//UV
//bmp-section 
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_ADDRESS 0x76
#define SEALEVELPRESSURE_HPA 1013.25

 Adafruit_BMP280 bmp;
 
 unsigned long lastCheck = 0;
 const unsigned long checkInterval = 5000; 
//bmp-section

//dht_11_temp_humi
#include "DHT.h"

// Pin where the DHT11 is connected
#define DHTPIN 13       // GPIO 13
#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);
//dht_11_temp_humi
// Structure to send data (must match receiver)
typedef struct struct_message {
  float temp;
  float hum;
  unsigned int readingId;
  float windSpeed;
  float pressure;
  float altitude;
  float uvIndex;
} struct_message;

struct_message outgoingReadings;

// Receiver's MAC address (replace with actual receiver MAC)
uint8_t receiverMAC[] = {0xCC, 0xDB, 0xA7, 0x5A, 0xAC, 0xA0}; // Example only

unsigned int readingId = 0;
bool txOk = false;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  txOk = (status == ESP_NOW_SEND_SUCCESS);
}
//wind-section
const int analogPin = 34;  // A0 connected to GPIO34
const int digitalPin = 4;  // D0 connected to GPIO4
//wind-section

float lastValidTemp = 0;
float lastValidHum = 0;

// ==== Wi-Fi credentials ====
const char* ssid = "AndroidAP1";
const char* password = "xpma3782";
unsigned long lastWifiConnectionAttempt = 0;
const uint8_t CHANNEL = 6;              // <- fixed channel
const uint32_t WIFI_RETRY_PERIOD_MS = 10000;   // one attempt/min

void onWiFiEvent(WiFiEvent_t ev)
{
    switch (ev)
    {
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("[Wi-Fi] lost AP, stop scan & back to ch 6");

        /* 1. abort any channel-hop scan */
        esp_wifi_scan_stop();                    // ignore ERR if none

        /* 2. retune radio so ESP-NOW keeps working */
        esp_wifi_set_promiscuous(true);
        esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
        esp_wifi_set_promiscuous(false);
        break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.print("[Wi-Fi] IP obtained: ");
        Serial.println(WiFi.localIP());
        break;

    default:
        break;
    }
}

void tryWiFiOnce()
{
    Serial.println("[Wi-Fi] one attempt …");
    WiFi.begin(ssid, password, CHANNEL, nullptr, true);   // async; stays on ch 6
    lastWifiConnectionAttempt = millis();
}

void WifiReconnectionTask(void *parameter) {
  /*for (;;) {
    Serial.print("Inside Core 0 loop");
    if (WiFi.status() != WL_CONNECTED && millis() - lastWifiConnectionAttempt > 10000) {         // poll every 10 second
      lastWifiConnectionAttempt = millis();
      WiFi.begin(ssid, password);
      Serial.print("Connecting to WiFi. Next connection attempt in 10 sec.");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Wifi CONNECTED!");
    }
    vTaskDelay(3000 / portTICK_PERIOD_MS); // Wait 3 seconds
  }*/
}

// ==== Exponential Moving Average (EMA) ==== // smooths, keeps step response
float ema(float newVal, float &prev, float alpha = 0.2)
{
    prev = alpha * newVal + (1 - alpha) * prev;
    return prev;
}

bool readStableDHT(float &rT, float &rH)
{
    for (uint8_t attempt = 0; attempt < 3; ++attempt)
    {
        float t = dht.readTemperature();   // °C
        float h = dht.readHumidity();      // %
        if (!isnan(t) && !isnan(h) &&
            t > 0 && t < 60 &&           // sane range
            h >= 0  && h <= 100)
        {
            rT = t;
            rH = h;
            return true;                   // good sample
        }
        delay(100);                        // let sensor settle, then retry
    }
    return false;                          // 3 strikes → treat as missing
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("ESP32 Sender MAC Address: ");
  Serial.println(WiFi.macAddress());
  WiFi.onEvent(onWiFiEvent);
  WiFi.mode(WIFI_STA);

  tryWiFiOnce(); 
  lastWifiConnectionAttempt = millis();
  Serial.print("Connecting to WiFi");
  /*while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }*/
  uint8_t channel = WiFi.channel();
  Serial.println("");
  Serial.println("STEP 12: WiFi connecteed");
  Serial.print("WiFi channel is: ");
  Serial.println(channel);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(receiverMAC)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }
  }

  // wind-section
  pinMode(digitalPin, INPUT);
  // wind-section

  //bmp-section
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100);
  if (!bmp.begin(I2C_ADDRESS)) {
     Serial.println("[INIT ERROR] BMP280 not detected at 0x76. Check wiring.");
   } else {
     Serial.println("[INIT] BMP280 initialized successfully.");
   }
  //bmp-section

  //UV
  analogReadResolution(12); // ESP32 default is 12-bit ADC (0-4095)
  //UV  

  /*
  // Wifi reconnection task on Core 0
  xTaskCreatePinnedToCore(
    WifiReconnectionTask,        // Task function
    "WifiReconnectionTask",      // Task name
    1024,                 // Stack size
    NULL,                 // Task parameters
    0,                    // Priority
    NULL,                 // Task handle (optional)
    0                     // Core 0
  ); */
}

void loop() {
  outgoingReadings.readingId = readingId++;
  //dht 11
  float stableT, stableH;
  if (readStableDHT(stableT, stableH)) {  // range-checked + retry
        static float smoothedT = stableT, smoothedH = stableH;   // initialise once
        ema(stableT, smoothedT);                         // smooth
        ema(stableH, smoothedH);

        outgoingReadings.temp = smoothedT;
        outgoingReadings.hum  = smoothedH;
  }
  // else: skip this cycle, keep previous good data
  delay(2000);

  Serial.print("Hum is: "); 
  Serial.println(outgoingReadings.hum);

  //dht 11

  //wind-section
  int raw = analogRead(analogPin);
  static float smoothedWind;
  ema(raw, smoothedWind, 0.3);
  float voltage = (smoothedWind / 4095.0) * 3.3;  // Adjust if using 5V
  outgoingReadings.windSpeed = voltage * 3.0 * 3.6; 
  Serial.println("Wind speed is: "); 
  Serial.println(voltage * 3.0 * 3.6);      // Heuristic multiplier
  //wind-section

  //bmp-section
  if (millis() - lastCheck >= checkInterval) {
     lastCheck = millis();
     // Try to reinitialize if it ever failed
     if (!bmp.begin(I2C_ADDRESS)) {
       Serial.println("[ERROR] Sensor communication failed during runtime.");
       return;
     }
     delay(500);
     outgoingReadings.pressure = (bmp.readPressure() / 100.0F) / 1013.25; // atm
     outgoingReadings.altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
   }
  //bmp-section

  //UV
  int analogValue = analogRead(UV_SENSOR_PIN);
  static float smootheUV;
  ema(analogValue, smootheUV, 0.2);
  // Convert analog reading (0-4095) to voltage (0-3.3V)
  float voltage_UV = (smootheUV / 4095.0) * 3.3;

  // UV index estimation
  // We map voltage to UV index based on their relation
  outgoingReadings.uvIndex = (voltage_UV - 0.99) * (15.0 / (2.8 - 0.99)); 
  if (outgoingReadings.uvIndex < 0) outgoingReadings.uvIndex = 0; // Clamp to zero
  //UV
  Serial.print("txOk is: ");
  Serial.println(txOk);
  txOk = false;
  // Send the data
  for (int attempt = 0; attempt < 10 && !txOk; ++attempt) {
    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&outgoingReadings, sizeof(outgoingReadings));
    uint32_t tStart = millis();
    while (!txOk && millis() - tStart < 200) {
        delay(0);                               // yield to Wi-Fi/ESP-NOW task
    }
    Serial.println("INSIDE THE LOOP");
    if (result == ESP_OK) {                    // handed to MAC layer
      Serial.println("Sent with success");
    } else {
      Serial.println("Error sending the data");
    } 
    if (txOk) {
      break;
    } else {
      delay(20);  
    }                   // short back-off
  }

  /*if (WiFi.status() != WL_CONNECTED && millis() - lastWifiConnectionAttempt > 10000) {         // poll every 10 second
    lastWifiConnectionAttempt = millis();
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi. Next connection attempt in 10 sec.");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wifi CONNECTED!");
  }*/

  //Serial.println(outgoingReadings.hum);
  delay(500); // Send every 1 seconds
}