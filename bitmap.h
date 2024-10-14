/*   bmp.h   */

#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>

typedef unsigned int color_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t file_size;
    color_t *pixels;
} bmp_t;

bmp_t bmp_init(size_t width, size_t height);
bmp_t bmp_load(char *filename);
void bmp_free(bmp_t bmp);
void bmp_set_pixel(bmp_t bmp, int x, int y, color_t color);
void bmp_write(bmp_t bmp, const char *filename);

#endif // BITMAP_H
