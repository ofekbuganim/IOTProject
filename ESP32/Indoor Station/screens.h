#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *outdoor_station;
    lv_obj_t *indoor_station;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *indoor_button;
    lv_obj_t *outdoor_button;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *label_temperature_4;
    lv_obj_t *obj9;
    lv_obj_t *label_humidity_4;
    lv_obj_t *label_temperature_3;
    lv_obj_t *label_hunidity_3;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *obj12;
    lv_obj_t *label_pressure;
    lv_obj_t *label_windspeed;
    lv_obj_t *label_uv_index;
    lv_obj_t *label_pressure_2;
    lv_obj_t *label_windspeed_2;
    lv_obj_t *label_uv_index_2;
    lv_obj_t *obj13;
    lv_obj_t *label_altitude;
    lv_obj_t *label_altitude_2;
    lv_obj_t *outdoor_return_button;
    lv_obj_t *obj14;
    lv_obj_t *obj15;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *label_temperature_2;
    lv_obj_t *obj18;
    lv_obj_t *label_humidity_2;
    lv_obj_t *label_temperature;
    lv_obj_t *label_hunidity;
    lv_obj_t *indoor_return_button;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_OUTDOOR_STATION = 2,
    SCREEN_ID_INDOOR_STATION = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_outdoor_station();
void tick_screen_outdoor_station();

void create_screen_indoor_station();
void tick_screen_indoor_station();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/