/*
  ESP32 ESP-NOW Sender
  Sends temperature, humidity, and reading ID to a receiver via ESP-NOW
*/

#include <esp_now.h>
#include <WiFi.h>

// Structure to send data (must match receiver)
typedef struct struct_message {
  int id;
  float temp;
  float hum;
  unsigned int readingId;
} struct_message;

struct_message outgoingReadings;

// Receiver's MAC address (replace with actual receiver MAC)
uint8_t receiverMAC[] = {0xCC, 0xDB, 0xA7, 0x5A, 0xAC, 0xA0}; // Example only

unsigned int readingId = 0;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("ESP32 Sender MAC Address: ");
  Serial.println(WiFi.macAddress());

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
}

void loop() {
  // Fill in some dummy data (replace with real sensor reads)
  outgoingReadings.id = 1;
  outgoingReadings.temp = 10.0; // e.g. 20.0 to 30.0
  outgoingReadings.hum = 10.0;  // e.g. 40.0 to 70.0
  outgoingReadings.readingId = readingId++;

  // Send the data
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&outgoingReadings, sizeof(outgoingReadings));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(5000); // Send every 5 seconds
}
