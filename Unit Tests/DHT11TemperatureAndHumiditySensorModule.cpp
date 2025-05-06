/*
 *  DHT11 Sensor                  ESP32 DevKit v1
 *   S (left) ------------------>  GPIO 4
 *   Middle (VCC) -------------->  3.3V
 *   - (right) ----------------->  GND
 */

#include "DHT.h"

// Pin where the DHT11 is connected
#define DHTPIN 4       // GPIO 4
#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(2000);  // DHT11 needs about 2 seconds between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius by default

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
}
