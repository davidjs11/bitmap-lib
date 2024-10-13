/*   bmp.h   */

#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>

typedef unsigned int color_t;

typedef struct {
    size_t width;
    size_t height;
    color_t *pixels;
} bmp_t;

bmp_t bmp_init(size_t width, size_t height);
void bmp_free(bmp_t bmp);
void bmp_set_pixel(bmp_t bmp, int x, int y, color_t color);
void bmp_write(bmp_t bmp, const char *filename);

#endif // BITMAP_H
