// parameters.h
#ifndef PARAMETERS_H
#define PARAMETERS_H

// Wi-Fi credentials
#define WIFI_SSID "AndroidAP1"
#define WIFI_PASSWORD "xpma3782"

// ==== ESP-NOW Communication ====
uint8_t receiverMAC[] = {0xCC, 0xDB, 0xA7, 0x5A, 0xAC, 0xA0};  // Indoor unit MAC address

// Firebase details
#define FIREBASE_HOST "weather-station-iot-a5c40.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyDwduIRpiQTkVHuWjsbeNKSepu0kQ1pwWQ"
#define FCM_TOKEN "fZbX4N9aSlyjqDo8IpqEQV:APA91bFO4ZcLi9oJfqCKLLjIB1YYWTaaIXn5V2qmW-1ewilWd6fjBpTM38dshlLf6-URvJdOfMBD40tTUMqH7pgyLn_NvDmf4D_AhWtcwHjt_FrKhITKJgQ"


// Sensor configuration
#define DHTPIN 13           // GPIO connected to DHT11
#define DHTTYPE DHT11       // DHT sensor type

#define UV_SENSOR_PIN 35    // Analog pin connected to UV sensor
#define WIND_SENSOR_PIN 34  // Digital pin for MH magnetic wind sensor
#define WIND_SENSOR_ANALOG 36  // Analog pin for wind (optional fallback or second type)

#define I2C_SDA_PIN 21      // I2C SDA for BMP280, LCD
#define I2C_SCL_PIN 22      // I2C SCL for BMP280, LCD


// Wind calibration constants
#define WIND_FACTOR 1.2     // Multiplier for converting pulses to m/s

#endif
