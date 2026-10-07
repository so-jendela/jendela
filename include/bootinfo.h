#ifndef JENDELA_BOOTINFO_H
#define JENDELA_BOOTINFO_H

#include <stdint.h>

typedef enum {
    JENDELA_PIXEL_RGB = 0,
    JENDELA_PIXEL_BGR = 1
} JendelaPixelFormat;

typedef struct {
    uint64_t base;
    uint64_t size;

    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;

    JendelaPixelFormat pixel_format;
} FramebufferInfo;

typedef struct {
    FramebufferInfo framebuffer;
} BootInfo;

#endif