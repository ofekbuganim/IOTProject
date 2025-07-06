#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <HTTPClient.h>
#include <time.h>
#include "ui.h"
// === Display & Touch Config ===
#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240
#define TOUCH_CS_PIN   13
#define TOUCH_MIN_X    203
#define TOUCH_MAX_X    3896
#define TOUCH_MIN_Y    176
#define TOUCH_MAX_Y    3721

//bool isOutdoorConnected = false;
//unsigned int currentReadingId = 0;

// Pin where the DHT11 is connected
#define DHTPIN 14       // GPIO 4
#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);
// Structure to receive data (must match sender)
typedef struct struct_message {
  float temp;
  float hum;
  unsigned int readingId;
  float windSpeed;
  float pressure;
  float altitude;
  float uvIndex;
} struct_message;

struct_message incomingReadings;

unsigned long lastOutdoorUpdateTime = 0;
const unsigned long timeoutThreshold = 5000; // 10 seconds

// == Callback when data is received ==
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Packet received from: ");
  Serial.println(macStr);
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  lastOutdoorUpdateTime = millis();  // Update last time data was received
  Serial.print("ESP-NOW OnDataRecv function running on core: ");
  Serial.println(xPortGetCoreID());  // 0 or 1

}

TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(TOUCH_CS_PIN);

// === LVGL 9.x draw buffer and display
static lv_color_t buf1[SCREEN_WIDTH * 10];
lv_display_t* display;

// === LVGL Flush Callback
void my_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1,
                    area->x2 - area->x1 + 1,
                    area->y2 - area->y1 + 1);
  tft.pushColors((uint16_t*)px_map,
                 (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1),
                 true);
  tft.endWrite();
  lv_display_flush_ready(disp);
}

// === Touch Read Callback
void touchpad_read_cb(lv_indev_t* indev, lv_indev_data_t* data) {
  //Serial.println("inside read cb !!");
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    uint16_t x = map(p.x, TOUCH_MAX_X, TOUCH_MIN_X, 0, SCREEN_WIDTH);
    uint16_t y = map(p.y, TOUCH_MAX_Y, TOUCH_MIN_Y, 0, SCREEN_HEIGHT);
    data->point.x = x;
    data->point.y = y;
    data->state = LV_INDEV_STATE_PRESSED;
    //Serial.printf("Touch: %d, %d\n", x, y);
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

// === Event Callbacks
static void indoor_btn_event_cb(lv_event_t *e) {
  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    lv_scr_load(objects.indoor_station);
  }
}

static void outdoor_btn_event_cb(lv_event_t *e) {
  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    lv_scr_load(objects.outdoor_station);
  }
}

static void return_btn_event_cb(lv_event_t *e) {
  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    lv_scr_load(objects.main);
  }
}

lv_obj_t *label_no_connection;

void createNoConnectionLabel(lv_obj_t *parent) {
  label_no_connection = lv_label_create(parent);
  lv_label_set_text(label_no_connection, "No connection\nto outdoor station");

  // Set size and alignment
  lv_obj_set_size(label_no_connection, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(label_no_connection, LV_ALIGN_TOP_MID, 5, 48);  // Slightly below the title

  // Style text
  lv_obj_set_style_text_font(label_no_connection, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(label_no_connection, lv_color_hex(0xFF5555), 0);  // soft red
  lv_obj_set_style_text_align(label_no_connection, LV_TEXT_ALIGN_CENTER, 0);

  // Background styling
  lv_obj_set_style_bg_color(label_no_connection, lv_color_hex(0x383842), 0);  // match screen background
  lv_obj_set_style_bg_opa(label_no_connection, LV_OPA_COVER, 0);
  lv_obj_set_size(label_no_connection, 310, 184);  // Fixed width and height
  //lv_obj_set_style_radius(label_no_connection, 8, 0);
  //lv_obj_set_style_border_width(label_no_connection, 2, 0);
  //lv_obj_set_style_border_color(label_no_connection, lv_color_hex(0xFF5555), 0);  // optional red border
  lv_label_set_long_mode(label_no_connection, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_pad_top(label_no_connection, 60, 0);
  lv_obj_set_style_pad_left(label_no_connection, 10, 0);
  lv_obj_set_style_pad_right(label_no_connection, 10, 0);

  // Start hidden
  lv_obj_add_flag(label_no_connection, LV_OBJ_FLAG_HIDDEN);
}

void fadeInLabel(lv_obj_t *obj) {
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);  // Show the label

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_values(&a, 0, 255);  // from transparent to fully visible
  lv_anim_set_time(&a, 500);       // duration in ms
  lv_anim_set_exec_cb(&a, [](void *obj, int32_t v) {
    lv_obj_set_style_opa((lv_obj_t *)obj, v, 0);
  });
  lv_anim_start(&a);
}

void fadeOutLabel(lv_obj_t *obj) {
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_values(&a, 255, 0);  // from opaque to transparent
  lv_anim_set_time(&a, 500);
  lv_anim_set_exec_cb(&a, [](void *obj, int32_t v) {
    lv_obj_set_style_opa((lv_obj_t *)obj, v, 0);
  });

  // Hide the label at the end of the animation
  lv_anim_set_ready_cb(&a, [](lv_anim_t *a) {
    lv_obj_add_flag((lv_obj_t *)a->var, LV_OBJ_FLAG_HIDDEN);
  });

  lv_anim_start(&a);
}

void showNoConnectionMessage() {
  if (label_no_connection)
    lv_obj_clear_flag(label_no_connection, LV_OBJ_FLAG_HIDDEN);
}

void hideNoConnectionMessage() {
  if (label_no_connection)
    lv_obj_add_flag(label_no_connection, LV_OBJ_FLAG_HIDDEN);
}

// ==== Wi-Fi credentials ====
const char* ssid = "Hodaya";
const char* password = "0526617158";
// ==== Firebase config ====
const char* firebase_project_id = "weather-station-iot-a5c40";
const char* firebase_api_key = "AIzaSyDwduIRpiQTkVHuWjsbeNKSepu0kQ1pwWQ";
// ==== Firestore REST API URL ====
String firestore_url = "https://firestore.googleapis.com/v1/projects/" + String(firebase_project_id) + "/databases/(default)/documents/sensor_data";

String getISO8601Time() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "2025-01-01T00:00:00Z";
  }
  char timeString[30];
  strftime(timeString, sizeof(timeString), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
  return String(timeString);
}

struct dataToFirestore {
  float temperature_outdoor;
  float temperature_indoor;
  float humidity_outdoor;
  float humidity_indoor;
  float wind_speed;
  float uv_index;
  float pressure;
  float altitude;
} dataToFirestore;

void sendSensorDataToFirestore() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Build Firestore JSON payload
    String payload = String("{\"fields\": {") +
      "\"temperature_outdoor\": {\"doubleValue\": " + String(dataToFirestore.temperature_outdoor, 2) + "}," +
      "\"temperature_indoor\": {\"doubleValue\": " + String(dataToFirestore.temperature_indoor, 2) + "}," +
      "\"humidity_outdoor\": {\"doubleValue\": " + String(dataToFirestore.humidity_outdoor, 2) + "}," +
      "\"humidity_indoor\": {\"doubleValue\": " + String(dataToFirestore.humidity_indoor, 2) + "}," +
      "\"wind_speed\": {\"doubleValue\": " + String(dataToFirestore.wind_speed, 2) + "}," +
      "\"uv_index\": {\"doubleValue\": " + String(dataToFirestore.uv_index, 2) + "}," +
      "\"pressure\": {\"doubleValue\": " + String(dataToFirestore.pressure, 2) + "}," +
      "\"altitude\": {\"doubleValue\": " + String(dataToFirestore.altitude, 2) + "}," +
      "\"timestamp\": {\"timestampValue\": \"" + getISO8601Time() + "\"}" +
    "}}";

    Serial.println("Sending payload to Firestore:");
    Serial.println(payload);
    http.begin(firestore_url + "?key=" + firebase_api_key);
    http.addHeader("Content-Type", "application/json");
    int response = http.POST(payload);

    Serial.print("Firestore POST status: ");
    Serial.println(response);

    if (response > 0) {
      String responseBody = http.getString();
      Serial.println("Response:");
      Serial.println(responseBody);
    } else {
      Serial.println("Error in sending POST");
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void firestoreTask(void *parameter) {
  for (;;) {
    sendSensorDataToFirestore();
    vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait 2 seconds
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("STEP 1: Boot started");
  dht.begin();
  Serial.println("STEP 2: DHT initialized");
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("STEP 3: ESP-NOW init failed");
    return;
  }
  Serial.println("STEP 3: ESP-NOW initialized");

  // Register receive callback
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("STEP 4: ESP-NOW recv callback registered");
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());

  // === Initialize LVGL
  lv_init();
  Serial.println("STEP 5: LVGL initialized");

  // === Initialize display
  tft.begin();
  Serial.println("STEP 6: TFT initialized");

  tft.setRotation(1);
  ts.begin();
  ts.setRotation(3);
  Serial.println("STEP 7: Touchscreen initialized");

  // === Create display object (LVGL 9)
  display = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_display_set_flush_cb(display, my_flush_cb);
  lv_display_set_buffers(display, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
  Serial.println("STEP 8: LVGL display configured");

  // === Create input device (touch)
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchpad_read_cb);
  lv_indev_set_disp(indev, display);
  Serial.println("STEP 9: LVGL touch input configured");

  // === Load UI from EEZ
  ui_init();
  lv_scr_load(objects.main);
  createNoConnectionLabel(objects.outdoor_station);
  Serial.println("STEP 10: UI initialized");

  // === Attach button event callbacks
  if (objects.indoor_button) lv_obj_add_event_cb(objects.indoor_button, indoor_btn_event_cb, LV_EVENT_CLICKED, NULL);
  if (objects.outdoor_button) lv_obj_add_event_cb(objects.outdoor_button, outdoor_btn_event_cb, LV_EVENT_CLICKED, NULL);
  if (objects.outdoor_return_button) {
    lv_obj_add_flag(objects.outdoor_return_button, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(objects.outdoor_return_button, return_btn_event_cb, LV_EVENT_CLICKED, NULL);
  }

  if (objects.indoor_return_button) {
    lv_obj_add_flag(objects.indoor_return_button, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(objects.indoor_return_button, return_btn_event_cb, LV_EVENT_CLICKED, NULL);
  }

  Serial.println("STEP 11: Event handlers registered");


  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("STEP 12: WiFi connecteed");

  // Set up time for timestamps
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  
  // Start Firestore HTTP task on Core 0
  xTaskCreatePinnedToCore(
    firestoreTask,        // Task function
    "FirestoreTask",      // Task name
    8192,                 // Stack size
    NULL,                 // Task parameters
    1,                    // Priority
    NULL,                 // Task handle (optional)
    0                     // Core 0
  );

}

void loop() {
  lv_task_handler();  // let the GUI do its work
  lv_tick_inc(5);     // tell LVGL how much time has passed
  delay(5);
  
  // === write to indoor screen
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius by default

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  lv_label_set_text(objects.label_temperature_2, (String(temperature) + "°C").c_str());
  lv_label_set_text(objects.label_humidity_2, (String(humidity) + "%").c_str());

  // === write to outdoor screen
  lv_label_set_text(objects.label_temperature_4, (String(incomingReadings.temp) + "°C").c_str());
  lv_label_set_text(objects.label_humidity_4, (String(incomingReadings.hum) + "%").c_str());
  lv_label_set_text(objects.label_pressure_2, (String(incomingReadings.pressure) + " atm").c_str());
  lv_label_set_text(objects.label_altitude_2, (String(incomingReadings.altitude) + " m").c_str());
  lv_label_set_text(objects.label_windspeed_2, (String(incomingReadings.windSpeed) + " km/h").c_str());
  if (incomingReadings.uvIndex < 3) {
    lv_label_set_text(objects.label_uv_index_2, "Low"); //Minimal protection needed
  } else if (incomingReadings.uvIndex < 6) {
    lv_label_set_text(objects.label_uv_index_2, "Moderate"); //Wear sunglasses and sunscreen
  } else if (incomingReadings.uvIndex < 8) {
    lv_label_set_text(objects.label_uv_index_2, "High"); //Reduce time in sun between 10am–4pm
  } else if (incomingReadings.uvIndex < 11) {
    lv_label_set_text(objects.label_uv_index_2, "Very High"); //Seek shade, wear protective clothing
  } else {
    lv_label_set_text(objects.label_uv_index_2, "Extreme"); //Avoid sun exposure
  }

  /*
  if (millis() - lastOutdoorUpdateTime > timeoutThreshold) {
    showNoConnectionMessage();
  } else {
    hideNoConnectionMessage();
  }
  */

  if (millis() - lastOutdoorUpdateTime > timeoutThreshold) {
    if (!lv_obj_has_flag(label_no_connection, LV_OBJ_FLAG_HIDDEN)) {
      // Already visible — do nothing
    } else {
      fadeInLabel(label_no_connection);  // Fade it in
    }
  } else {
    if (!lv_obj_has_flag(label_no_connection, LV_OBJ_FLAG_HIDDEN)) {
      fadeOutLabel(label_no_connection); // Fade it out
    }
  }

  dataToFirestore.temperature_outdoor = incomingReadings.temp;
  dataToFirestore.temperature_indoor = temperature;
  dataToFirestore.humidity_outdoor = incomingReadings.hum;
  dataToFirestore.humidity_indoor = humidity;
  dataToFirestore.wind_speed = incomingReadings.windSpeed;
  dataToFirestore.uv_index = incomingReadings.uvIndex;
  dataToFirestore.pressure = incomingReadings.pressure;
  dataToFirestore.altitude = incomingReadings.altitude;

}