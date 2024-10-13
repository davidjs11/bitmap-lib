/*   bmp.c   */

#include <bitmap.h>
#include <stdlib.h>
#include <fcntl.h>
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
    uint32_t file_width;
    uint32_t file_height;
    uint16_t color_planes;
    uint16_t bit_depth;
    uint32_t compression;
    uint32_t image_size;
    uint32_t ppm_horizontal;
    uint32_t ppm_vertical;
    uint32_t num_colors;
    uint32_t important_colors;
} __attribute__((packed)) bmp_infoheader_t;


bmp_t bmp_init(size_t width, size_t height) {
    bmp_t bmp;
    bmp.width = width;
    bmp.height = height;
    bmp.pixels = (color_t *) malloc(width*height*sizeof(color_t));
    return bmp;
}

void bmp_free(bmp_t bmp) {
    free(bmp.pixels);
    bmp.pixels = NULL;
}

int bmp_rowsize(bmp_t bmp) {
    int row_length = 3 * bmp.width;
    if (row_length % 4)
        row_length += 4 - (row_length % 4);
    return row_length;
}

int bmp_imagesize(bmp_t bmp) {
    return bmp_rowsize(bmp) * bmp.height;
}

bmp_fileheader_t bmp_fileheader(bmp_t bmp) {
    bmp_fileheader_t header = {
        .file_size = 14 + 40 + bmp_imagesize(bmp),
        .reserved = 0,
        .offset = 14 + 40,
    };
    strncpy(header.header_field, "BM", 2);
    printf("%d\n", bmp_imagesize(bmp));

    return header;
}

bmp_infoheader_t bmp_infoheader(bmp_t bmp) {
    bmp_infoheader_t header = {
        .header_size = 40,
        .file_width = bmp.width,
        .file_height = bmp.height,
        .color_planes = 1,
        .bit_depth = 24,
        .compression = 0,
        .image_size = bmp_imagesize(bmp),
        .ppm_horizontal = 0,
        .ppm_vertical = 0,
        .num_colors = 0,
        .important_colors = 0,
    };

    return header;
}

void bmp_write(bmp_t bmp, const char *filename) {
    int fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
    if (fd < 0) exit(EXIT_FAILURE);

    // write headers
    bmp_fileheader_t fileheader = bmp_fileheader(bmp);
    bmp_infoheader_t infoheader = bmp_infoheader(bmp);
    write(fd, &fileheader, sizeof(fileheader));
    write(fd, &infoheader, sizeof(infoheader));

    // write pixel data
    int row_size = bmp_rowsize(bmp);
    for (int y = bmp.height - 1; y >= 0; y--) {
        for (int x = 0; x < bmp.width; x++) {
            color_t color = bmp.pixels[y * bmp.width + x];
            uint8_t red = (color & 0xFF0000) >> 16;
            uint8_t green = (color & 0x00FF00) >> 8;
            uint8_t blue = (color & 0x0000FF);
            write(fd, &blue, 1);
            write(fd, &green, 1);
            write(fd, &red, 1);
        }

        printf("row size: %d\n", row_size);
        for (int i = bmp.width * 3; i < row_size; i++) {
            printf("o");
            char null = 0x00;
            write(fd, &null, 1);
        }
        printf("\n");
    }

    close(fd);
}

void bmp_set_pixel(bmp_t bmp, int x, int y, color_t color) {
    if (0 <= x && x < bmp.width && 0 <= y && y < bmp.height)
        bmp.pixels[y * bmp.width + x] = color;
}
