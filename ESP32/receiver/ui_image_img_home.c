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

#ifndef LV_ATTRIBUTE_IMG_IMG_HOME
#define LV_ATTRIBUTE_IMG_IMG_HOME
#endif

static const
LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_HOME
uint8_t img_img_home_map[] = {

    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xf0,0xf0,0xf0,0x10,0xfe,0xfe,0xfe,0xa6,0xfe,0xfe,0xfe,0xa5,0xf0,0xf0,0xf0,0x10,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfc,0xfc,0xfc,0x43,0xfe,0xfe,0xfe,0xe3,0xfe,0xfe,0xfe,0xfa,0xfe,0xfe,0xfe,0xfa,0xfe,0xfe,0xfe,0xe1,0xfc,0xfc,0xfc,0x40,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xdb,0xdb,0xdb,0x07,0xfe,0xfe,0xfe,0x8d,0xfe,0xfe,0xfe,0xfd,0xfe,0xfe,0xfe,0xd7,0xfb,0xfb,0xfb,0x33,0xfb,0xfb,0xfb,0x33,0xfe,0xfe,0xfe,0xd7,0xfe,0xfe,0xfe,0xfd,0xfe,0xfe,0xfe,0x8f,0xe3,0xe3,0xe3,0x09,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfa,0xfa,0xfa,0x2d,0xfe,0xfe,0xfe,0xd2,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0x95,0xe6,0xe6,0xe6,0x0a,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xe6,0xe6,0xe6,0x0a,0xfe,0xfe,0xfe,0x95,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xd5,0xfa,0xfa,0xfa,0x31,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0x80,0x80,0x80,0x02,0xfd,0xfd,0xfd,0x72,0xfe,0xfe,0xfe,0xf7,0xfe,0xfe,0xfe,0xe8,0xfc,0xfc,0xfc,0x4a,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfc,0xfc,0xfc,0x4a,0xfe,0xfe,0xfe,0xe9,0xfe,0xfe,0xfe,0xf9,0xfd,0xfd,0xfd,0x76,0x80,0x80,0x80,0x02,
    0xfe,0xfe,0xfe,0xb3,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xfe,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xb9,
    0xfe,0xfe,0xfe,0x92,0xfd,0xfd,0xfd,0x62,0xfe,0xfe,0xfe,0xe0,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xde,0xfd,0xfd,0xfd,0x5d,0xfe,0xfe,0xfe,0x96,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xdf,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xde,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xdf,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xde,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xdf,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xfb,0xfb,0xfb,0x3a,0xfe,0xfe,0xfe,0xa7,0xfe,0xfe,0xfe,0xa7,0xfe,0xfe,0xfe,0xa7,0xfe,0xfe,0xfe,0xa7,0xfb,0xfb,0xfb,0x39,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xde,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xdf,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0x80,0xfe,0xfe,0xfe,0xee,0xfe,0xfe,0xfe,0xa7,0xfe,0xfe,0xfe,0xa7,0xfe,0xfe,0xfe,0xee,0xfd,0xfd,0xfd,0x7f,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xde,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xdf,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0x80,0xfe,0xfe,0xfe,0xcd,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xce,0xfd,0xfd,0xfd,0x7f,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xde,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xdf,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0x80,0xfe,0xfe,0xfe,0xcd,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xce,0xfd,0xfd,0xfd,0x7f,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xde,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xdf,0xfe,0xfe,0xfe,0x9f,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0x80,0xfe,0xfe,0xfe,0xcd,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xce,0xfd,0xfd,0xfd,0x7f,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xa0,0xfe,0xfe,0xfe,0xde,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xdf,0xfe,0xfe,0xfe,0xcf,0xfe,0xfe,0xfe,0x80,0xfe,0xfe,0xfe,0xc0,0xfe,0xfe,0xfe,0xe6,0xfe,0xfe,0xfe,0x80,0xfe,0xfe,0xfe,0x80,0xfe,0xfe,0xfe,0xe6,0xfe,0xfe,0xfe,0xbf,0xfe,0xfe,0xfe,0x80,0xfe,0xfe,0xfe,0xd0,0xfe,0xfe,0xfe,0xde,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,
    0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,0xfe,0xfe,0xfe,0xc1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xc0,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0x00,

};

const lv_image_dsc_t img_img_home = {
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.cf = LV_COLOR_FORMAT_ARGB8888,
  .header.flags = 0,
  .header.w = 16,
  .header.h = 16,
  .header.stride = 64,
  .data_size = sizeof(img_img_home_map),
  .data = img_img_home_map,
};

