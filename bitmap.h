/*   bmp.h   */

#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t color_t;

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} __attribute__((packed)) pixel_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t file_size;
    pixel_t *pixels;
} bmp_t;

bmp_t bmp_create(size_t width, size_t height);
void bmp_free(bmp_t bmp);
bmp_t bmp_load(char *filename);
void bmp_write(bmp_t bmp, const char *filename);
void bmp_set_pixel(bmp_t bmp, int x, int y, uint32_t color);
color_t bmp_get_pixel(bmp_t bmp, int x, int y);

#endif // BITMAP_H
