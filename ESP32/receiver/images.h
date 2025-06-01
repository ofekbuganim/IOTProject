#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_img_home;
extern const lv_img_dsc_t img_img_back;
extern const lv_img_dsc_t img_icon_temp_22p;
extern const lv_img_dsc_t img_icon_humd_22p;
extern const lv_img_dsc_t img_icon_wind_22p;
extern const lv_img_dsc_t img_icon_pressure_22p;
extern const lv_img_dsc_t img_icon_uv_22p;
extern const lv_img_dsc_t img_icons8_sun_15;
extern const lv_img_dsc_t img_icons8_air_15;
extern const lv_img_dsc_t img_icons8_pressure_15;
extern const lv_img_dsc_t img_icons8_mountain_15;
extern const lv_img_dsc_t img_icons8_height_15;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[12];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/