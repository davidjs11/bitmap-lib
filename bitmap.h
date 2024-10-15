/*   bmp.h   */

#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>

/* useful macro */
#define   RGB(r,g,b) (((r&0xFF)<<16) | ((g&0xFF)<<8) | (b&0xFF))
#define RGB32(r,g,b) (((r&0xFF)<<16) | ((g&0xFF)<<8) | (b&0xFF))


/* types and structure definition */
typedef uint32_t color_t;

typedef struct {
    uint8_t     b;
    uint8_t     g;
    uint8_t     r;
} __attribute__((packed)) pixel_t;

typedef struct {
    uint32_t    width;
    uint32_t    height;
    uint32_t    file_size;
    pixel_t     *pixels;
} bmp_t;


/* functions */
bmp_t   bmp_create(size_t width, size_t height);
bmp_t   bmp_load(const char *filename);
void    bmp_free(bmp_t bmp);
void    bmp_write(bmp_t bmp, const char *filename);
void    bmp_dwrite(bmp_t bmp, int fd);
void    bmp_set_pixel(bmp_t bmp, size_t x, size_t y, uint32_t color);
color_t bmp_get_pixel(bmp_t bmp, size_t x, size_t y);

#endif // BITMAP_H
