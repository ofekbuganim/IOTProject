## 🌤️ Smart Weather Station Project by :  
Ofek Buganim & Keren Ma'avari & Lidor Dadon
Group #: 8

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

## 🔌 Connection Diagram
<img width="240" alt="six" src="https://github.com/user-attachments/assets/202ffd14-0242-4ebd-8fc8-873fdfc48b05" />

---

## 🖼️ Project Poster
[8.Smart.Weather.Station.IOT.pdf](https://github.com/user-attachments/files/21205365/8.Smart.Weather.Station.IOT.pdf)


---

This project is part of ICST - The Interdisciplinary Center for Smart Technologies, Taub Faculty of Computer Science, Technion
https://icst.cs.technion.ac.il/
