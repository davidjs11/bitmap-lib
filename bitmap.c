/*   bitmap.c   */

#include <bitmap.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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

/* private functions */
size_t bmp_rowsize(size_t width, size_t height) {
    size_t row_size = width * 3;
    if (row_size % 4) row_size += 4 - (row_size % 4);
    return row_size;
}

size_t bmp_imagesize(size_t width, size_t height) {
    return bmp_rowsize(width, height) * height;
}

size_t bmp_get_index(bmp_t bmp, int x, int y) {
    size_t row = (bmp.height - y - 1);
    size_t col = 3 * x;
    return row * bmp_rowsize(bmp.width, bmp.height) + col;
}

/* public functions */
bmp_t bmp_create(size_t width, size_t height) {
    bmp_t bmp;
    bmp.width = width;
    bmp.height = height;
    bmp.pixels = (pixel_t *) malloc(bmp_imagesize(width, height));
    return bmp;
}

bmp_t bmp_load(char *filename) {
    int fd = open(filename, O_RDWR, 0);
    if (fd < 0) exit(EXIT_FAILURE);

    /* load headers */
    bmp_fileheader_t fileheader;
    read(fd, &fileheader, sizeof(fileheader));

    // if info header is not 40 bytes long,
    // discard it (only bmp v1 is suported)
    if (fileheader.offset - sizeof(fileheader) != 40)
        exit(EXIT_FAILURE);

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
    if (fd < 0) exit(EXIT_FAILURE);

    // write file header
    bmp_fileheader_t fileheader = {
        .file_size = 14 + 40 + bmp_imagesize(bmp.width, bmp.height),
        .reserved = 0,
        .offset = 14 + 40,
    };
    strncpy(fileheader.header_field, "BM", 2);
    write(fd, &fileheader, sizeof(fileheader));

    // write info header
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
    write(fd, &infoheader, sizeof(infoheader));

    // write pixel data
    int rowsize = bmp_rowsize(bmp.width, bmp.height);
    for (int y = bmp.height-1; y >= 0; y--) {
        for (int x = 0; x < bmp.width; x++) {
            write(fd, &(bmp.pixels[bmp_get_index(bmp, x, y)]), sizeof(pixel_t));
        }

        for (int i = bmp.width * 3; i < rowsize; i++) {
            char null = 0x00;
            write(fd, &null, 1);
        }
    }

    close(fd);
}

color_t bmp_get_pixel(bmp_t bmp, int x, int y) {
    color_t color;
    // there would be an assert
    if (0 <= x && x < bmp.width && 0 <= y && y < bmp.height) {
        size_t index = bmp_get_index(bmp, x, y);
        pixel_t pixel = bmp.pixels[index];
        uint8_t red = (pixel.r & 0xFF);
        uint8_t green = (pixel.g & 0xFF);
        uint8_t blue = (pixel.b & 0xFF);
        color = (red << 16) | (green << 8) | blue;
    }
    return color;
}

void bmp_set_pixel(bmp_t bmp, int x, int y, uint32_t color) {
    // also assert
    if (0 <= x && x < bmp.width && 0 <= y && y < bmp.height) {
        size_t index = bmp_get_index(bmp, x, y);
        bmp.pixels[index].r = (color & 0xFF0000) >> 16;
        bmp.pixels[index].g = (color & 0x00FF00) >>  8;
        bmp.pixels[index].b = (color & 0x0000FF) >>  0;
    }
}
