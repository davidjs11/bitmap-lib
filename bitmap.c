/*   bitmap.c   */

#include <bitmap.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


/* --- custom assert flex ;) --- */
#define ASSERT(cond, ...) {\
    if(!(cond)) {\
        fprintf(stderr,__VA_ARGS__);\
        exit(EXIT_FAILURE);\
    }\
}


/* --- private structures --- */
typedef struct {
    char header_field[2];
    uint32_t file_size;
    uint32_t reserved;
    uint32_t offset;
} __attribute__((packed)) bmp_fileheader_t;

typedef struct {
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes;
    uint16_t bit_depth;
    uint32_t compression;
    uint32_t image_size;
    uint32_t ppm_horizontal;
    uint32_t ppm_vertical;
    uint32_t num_colors;
    uint32_t important_colors;
} __attribute__((packed)) bmp_infoheader_t;


/* --- private functions --- */
size_t bmp_rowsize(size_t width, size_t height) {
    size_t row_size = width * 3;
    if (row_size % 4) row_size += 4 - (row_size % 4);
    return row_size;
}

size_t bmp_imagesize(size_t width, size_t height) {
    return bmp_rowsize(width, height) * height;
}

size_t bmp_get_index(bmp_t bmp, int x, int y) {
    // get index in bytes
    size_t row = (bmp.height - y - 1);
    size_t col = 3 * x;
    return row * bmp_rowsize(bmp.width, bmp.height) + col;
}


/* --- private functions --- */
bmp_t bmp_create(size_t width, size_t height) {
    bmp_t bmp;
    bmp.width = width;
    bmp.height = height;
    bmp.pixels = (pixel_t *) malloc(bmp_imagesize(width, height));
    return bmp;
}

bmp_t bmp_load(const char *filename) {
    int fd = open(filename, O_RDWR, 0);
    ASSERT(fd >= 0, "[-] open(%s, ...)\n", filename);

    /* load headers */
    bmp_fileheader_t fileheader;
    read(fd, &fileheader, sizeof(fileheader));

    // if info header is not 40 bytes long,
    // discard it (only bmp v1 is suported)
    ASSERT(fileheader.offset - sizeof(fileheader) == 40,
           "[-] only bitmap DIBv1 is supported\n");

    bmp_infoheader_t infoheader;
    read(fd, &infoheader, sizeof(infoheader));

    /* load pixel data into memory */
    bmp_t bmp = bmp_create(infoheader.width, infoheader.height);
    read(fd, bmp.pixels, infoheader.image_size);

    return bmp;
}

void bmp_free(bmp_t bmp) {
    if (bmp.pixels) {
        free(bmp.pixels);
        bmp.pixels = NULL;
    }
}

void bmp_write(bmp_t bmp, const char *filename) {
    int fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
    ASSERT(fd >= 0, "[-] open(%s, ...).\n", filename);
    
    bmp_dwrite(bmp, fd);
}

void bmp_dwrite(bmp_t bmp, int fd) {
    /* create headers */
    bmp_fileheader_t fileheader = {
        .file_size = 14 + 40 + bmp_imagesize(bmp.width, bmp.height),
        .reserved = 0,
        .offset = 14 + 40,
    };
    strncpy(fileheader.header_field, "BM", 2);

    bmp_infoheader_t infoheader = {
        .header_size = 40,
        .width = bmp.width,
        .height = bmp.height,
        .color_planes = 1,
        .bit_depth = 24,
        .compression = 0,
        .image_size = bmp_imagesize(bmp.width, bmp.height),
        .ppm_horizontal = 0,
        .ppm_vertical = 0,
        .num_colors = 0,
        .important_colors = 0,
    };

    /* write data */
    write(fd, &fileheader, sizeof(fileheader));
    write(fd, &infoheader, sizeof(infoheader));
    write(fd, bmp.pixels, bmp_imagesize(bmp.width, bmp.height));

    close(fd);
}

color_t bmp_get_pixel(bmp_t bmp, size_t x, size_t y) {
    ASSERT(0 <= x && x < bmp.width && 0 <= y && y < bmp.height,
           "[-] bmp_get_pixel(%u, %u): invalid position.\n", x, y);

    pixel_t pixel;
    size_t index = bmp_get_index(bmp, x, y);
    memcpy(&pixel, ((uint8_t *) bmp.pixels)+index, sizeof(pixel));

    return ((pixel.r << 16) | (pixel.g << 8) | pixel.b);
}

void bmp_set_pixel(bmp_t bmp, size_t x, size_t y, uint32_t color) {
    ASSERT(0 <= x && x < bmp.width && 0 <= y && y < bmp.height,
           "[-] bmp_set_pixel(%u, %u): invalid position.\n", x, y);

    pixel_t pixel = {
        .r = (color & 0xFF0000) >> 16,
        .g = (color & 0x00FF00) >>  8,
        .b = (color & 0x0000FF) >>  0
    };

    size_t index = bmp_get_index(bmp, x, y);
    memcpy(((uint8_t *) bmp.pixels)+index, &pixel, sizeof(pixel));
}
