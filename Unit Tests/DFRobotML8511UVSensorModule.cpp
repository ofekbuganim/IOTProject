#define UV_SENSOR_PIN 34  // D connected to A

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // ESP32 default is 12-bit ADC (0-4095)
}

void loop() {
  int analogValue = analogRead(UV_SENSOR_PIN);
  
  // Convert analog reading (0-4095) to voltage (0-3.3V)
  float voltage = (analogValue / 4095.0) * 3.3;

  // UV index estimation
  // We map voltage to UV index based on their relation
  float uvIndex = (voltage - 0.99) * (15.0 / (2.8 - 0.99)); 
  if (uvIndex < 0) uvIndex = 0; // Clamp to zero
  
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | Estimated UV Index: ");
  Serial.println(uvIndex, 2);
  
  delay(1000); // Read once per second
}
