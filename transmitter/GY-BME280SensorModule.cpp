#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define I2C_SDA 21
#define I2C_SCL 22
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

void setup() {
  Serial.begin(115200);
  delay(1000); // Give time for Serial Monitor to open
  
  Wire.begin(I2C_SDA, I2C_SCL); // Initialize I2C with ESP32 pins
  
  if (!bme.begin(0x76)) {  // We verified it is 0x76
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1); // Freeze
  }
  
  Serial.println("BME280 Initialized Successfully!");
}

void loop() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
  delay(2000); // Wait 2 seconds between readings
}
