/*
 *  BME280 Sensor                  ESP32 DevKit v1
 *   VCC ------------------------>  3V3
 *   GND ------------------------>  GND
 *   SCL ------------------------>  GPIO 22
 *   SDA ------------------------>  GPIO 21
 */

 #include <Wire.h>
 #include <Adafruit_BMP280.h>
 
 #define SDA_PIN 21
 #define SCL_PIN 22
 #define I2C_ADDRESS 0x76
 #define SEALEVELPRESSURE_HPA 1013.25
 
 Adafruit_BMP280 bmp;
 
 unsigned long lastCheck = 0;
 const unsigned long checkInterval = 5000;
 
 unsigned int initFailures = 0;
 unsigned int readFailures = 0;
 unsigned int successReads = 0;
 
 // Assertion utility
 void assertTest(bool condition, const char* label) {
   Serial.print("[ASSERT] ");
   Serial.print(label);
   if (condition) {
     Serial.println(" -> ✅ PASS");
   } else {
     Serial.println(" -> ❌ FAIL");
   }
 }
 
 void setup() {
   Serial.begin(115200);
   delay(1000);
 
   Serial.println("\n--- BMP280 CONTINUOUS UNIT TEST ---");
 
   Wire.begin(SDA_PIN, SCL_PIN);
   delay(100);
 
   if (!bmp.begin(I2C_ADDRESS)) {
     Serial.println("[INIT ERROR] BMP280 not detected at 0x76. Check wiring.");
     initFailures++;
   } else {
     Serial.println("[INIT] BMP280 initialized successfully.");
   }
 }
 
 void loop() {
   if (millis() - lastCheck >= checkInterval) {
     lastCheck = millis();
 
     Serial.println("\n[READ CYCLE] ------------------------------");
 
     // Try to reinitialize if it ever failed
     if (!bmp.begin(I2C_ADDRESS)) {
       Serial.println("[ERROR] Sensor communication failed during runtime.");
       initFailures++;
       return;
     }
 
     float temp = bmp.readTemperature();
     float pressure = bmp.readPressure() / 100.0F; // hPa
     float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
 
     bool tempOK = isfinite(temp) && temp > -40 && temp < 85;
     bool pressureOK = isfinite(pressure) && pressure > 300 && pressure < 1100;
     bool altitudeOK = isfinite(altitude);
 
     // Assertions
     assertTest(tempOK, "Temperature in valid range (-40°C to 85°C)");
     assertTest(pressureOK, "Pressure in valid range (300 to 1100 hPa)");
     assertTest(altitudeOK, "Altitude is valid (not NaN)");
 
     if (tempOK && pressureOK && altitudeOK) {
       successReads++;
     } else {
       readFailures++;
     }
 
     // Print live values
     Serial.println("[DATA] Temperature: " + String(temp) + " °C");
     Serial.println("[DATA] Pressure: " + String(pressure) + " hPa");
     Serial.println("[DATA] Altitude: " + String(altitude) + " m");
 
     // Summary so far
     Serial.println("[STATS] Successful Reads: " + String(successReads));
     Serial.println("[STATS] Read Failures: " + String(readFailures));
     Serial.println("[STATS] Init Failures: " + String(initFailures));
   }
 }
 
