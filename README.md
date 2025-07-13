## 🌤️ Smart Weather Station Project by :  
Ofek Buganim & Keren Ma'avari & Lidor Dadon
Group #: 8

---

## Details about the project
This project is a **Smart Weather Station** composed of multiple components working together to monitor and display environmental conditions in real time. The system consists of:

### 1. **Outdoor Sensor Unit (ESP32)**
- Measures real-time environmental data including:
  - 🌡️ Temperature
  - 💧 Humidity
  - 🌞 UV Index
  - 🌬️ Wind Speed 
  - 📈 Barometric Pressure
  - 🏔️ Altitude
- Sends the data wirelessly using **ESP-NOW** protocol to the indoor unit.

### 2. **Indoor Display Unit (ESP32)**
- Measures real-time environmental data including:
  - 🌡️ Temperature
  - 💧 Humidity
- Receives data from the outdoor unit.
- Displays the following on an LCD  touch screen (TFT):
  - Outdoor temperature, humidity, UV Index, Wind Speed, Barometric Pressure, Altitude 
  - Indoor temperature and humidity

### 3. **Mobile App (Flutter)**
- Developed using **Flutter** framework.
- Displays:
  - Real-time sensor data synced via **Firebase**
  - Graphs and charts of historical data
  - Weather forecast using external weather API
  - Customizable alert settings for UV, temperature, wind, humidity and water plant
- Syncs data with Firebase for storage and access.

### 4. **Firebase Cloud Backend**
- Stores all sensor readings for history and analysis.
  
---

| Folder Name      | Description |
|------------------|-------------|
| `Documentation/` | Includes problems we encountered and how we solved them, along with the final connection diagram. |
| `ESP32/`         | Source code (firmware) for the ESP32 units – indoor and outdoor stations. |
| `Unit_Tests/`    | Hardware test scripts for individual sensors and components (e.g., DHT11, UV sensor, Firebase). |
| `flutter_app/`   | Flutter mobile application code written in Dart – includes real-time view, charts, forecasts, alerts, and settings. |
| `Weekly progress/`    | Weekly progress logs and tracking of development milestones throughout the project timeline. |

---

# 📦 Libraries & Versions

### ESP32 (Arduino Framework)

| Library Name               | Version   | Purpose                          |
|----------------------------|-----------|----------------------------------|
| `esp_now.h`               | Built-in  | Wireless communication between ESP32s |
| `WiFi.h`                  | Built-in  | WiFi connection to Firebase      |
| `esp_wifi.h`              | Built-in  | Advanced WiFi settings           |
| `DHT.h`                   | 1.4.2     | Read temperature and humidity    |
| `Adafruit_BMP280.h`       | 2.5.0     | Pressure & altitude sensor       |
| `Wire.h`                  | Built-in  | I2C communication                |
| `Adafruit_Sensor.h`       | 1.1.2     | Sensor base class                |
| `LiquidCrystal_I2C.h`     | 1.1.2     | LCD screen with I2C (indoor unit) |
| `lvgl.h` (LittlevGL)      | varies    | UI graphics engine for screen rendering |

### Flutter (Mobile App)

| Package Name              | Version       | Purpose                         |
|---------------------------|----------------|----------------------------------|
| `flutter`                | 3.22.1 (SDK)  | Core Flutter framework          |
| `firebase_core`          | ^2.30.0       | Initialize Firebase             |
| `cloud_firestore`        | ^4.17.0       | Realtime + historical data      |
| `firebase_messaging`     | ^14.7.0       | Push notifications              |
| `shared_preferences`     | ^2.2.2        | Save alert settings locally     |
| `intl`                   | ^0.18.1       | Time/date formatting            |
| `fl_chart`               | ^0.64.0       | Graph/chart rendering           |
| `connectivity_plus`      | ^5.0.2        | Detect internet/WiFi status     |

---

## 🔩 Hardware Components & Quantities

| Component                         | Quantity | Used In           | Notes |
|----------------------------------|----------|-------------------|-------|
| ESP32 DevKit v1                  | 2        | Indoor + Outdoor  | Main microcontroller |
| DHT11 Temperature & Humidity     | 2        | Indoor + Outdoor  | Measures temp/humidity |
| ML8511 UV Sensor                 | 1        | Outdoor           | UV index via analog |
| BMP280 Barometric Sensor         | 1        | Outdoor           | Pressure + Altitude |
| Wind Speed Sensor (MH + motor)   | 1        | Outdoor           | Based on magnetic pulses |
| LCD Screen 20x4 (I2C)            | 1        | Indoor            | Displays all readings |
| I2C Module for LCD               | 1        | Indoor            | For I2C connection to LCD |
| Wires, breadboards, resistors    | Many     | Both              | For circuit connections |

---

## 🔌 Connection Diagram
<img width="240" alt="six" src="https://github.com/user-attachments/assets/202ffd14-0242-4ebd-8fc8-873fdfc48b05" />

---

## 🖼️ Project Poster
[8.Smart.Weather.Station.IOT.pdf](https://github.com/user-attachments/files/21205365/8.Smart.Weather.Station.IOT.pdf)

---

This project is part of ICST - The Interdisciplinary Center for Smart Technologies, Taub Faculty of Computer Science, Technion
https://icst.cs.technion.ac.il/
