#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#elif defined(LV_BUILD_TEST)
#include "../lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_ICONS8_HEIGHT_15
#define LV_ATTRIBUTE_IMG_ICONS8_HEIGHT_15
#endif

static const
LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_ICONS8_HEIGHT_15
uint8_t img_icons8_height_15_map[] = {

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x71,0x4a,0x6f,0xff,0x6f,0x60,0x6f,0xff,0x6f,0x60,0x6f,0xff,0x6f,0x60,0x6f,0xff,0x6f,0x60,0x6f,0xff,0x6f,0x60,0x6e,0xff,0x6e,0x45,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0xff,0x70,0x34,0x6f,0xff,0x6f,0x30,0x6f,0xff,0x6f,0x30,0x71,0xff,0x6f,0x7e,0x6f,0xff,0x6f,0x30,0x6f,0xff,0x6f,0x30,0x72,0xff,0x6d,0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0xff,0x6e,0x2c,0x00,0x00,0x00,0x00,0x71,0xff,0x6e,0x61,0x70,0xff,0x6f,0xe1,0x71,0xff,0x6e,0x61,0x00,0x00,0x00,0x00,0x73,0xff,0x6e,0x2c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x70,0xff,0x70,0x68,0x71,0xff,0x6f,0x90,0x70,0xff,0x70,0x68,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x6f,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x6f,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0xff,0x6e,0x2c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x6f,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0xff,0x6e,0x2c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6d,0xff,0x6d,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x6f,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6d,0xff,0x6d,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0xff,0x6e,0x2c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x6f,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0xff,0x6e,0x2c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x6f,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x6f,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x67,0x71,0xff,0x6f,0x90,0x70,0xff,0x70,0x68,0x00,0x00,0x00,0x00,0x6f,0xff,0x6f,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0xff,0x6e,0x2c,0x00,0x00,0x00,0x00,0x71,0xff,0x6e,0x61,0x70,0xff,0x6f,0xe1,0x71,0xff,0x6e,0x61,0x00,0x00,0x00,0x00,0x73,0xff,0x6e,0x2c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6e,0xff,0x6e,0x35,0x6f,0xff,0x6f,0x30,0x6f,0xff,0x6f,0x30,0x70,0xff,0x70,0x7d,0x6f,0xff,0x6f,0x30,0x6f,0xff,0x6f,0x30,0x72,0xff,0x6d,0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0xff,0x71,0x4a,0x6f,0xff,0x6f,0x60,0x6f,0xff,0x6f,0x60,0x6f,0xff,0x6f,0x60,0x6f,0xff,0x6f,0x60,0x6f,0xff,0x6f,0x60,0x6e,0xff,0x6e,0x45,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};

const lv_image_dsc_t img_icons8_height_15 = {
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.cf = LV_COLOR_FORMAT_ARGB8888,
  .header.flags = 0,
  .header.w = 15,
  .header.h = 15,
  .header.stride = 60,
  .data_size = sizeof(img_icons8_height_15_map),
  .data = img_icons8_height_15_map,
};

