/*   un-rgb.c   */

/* 
 * a program that separates the three color channels (RGB) from a photo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bitmap.h>

int main(int argc, char *argv[]) {
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "usage: %s <photo.bmp>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* prepare filenames */
    char *filenames[3];
    filenames[0] = (char *) malloc(strlen(argv[1])+4+1);
    sprintf(filenames[0], "red_%s", argv[1]);
    filenames[1] = (char *) malloc(strlen(argv[1])+6+1);
    sprintf(filenames[1], "green_%s", argv[1]);
    filenames[2] = (char *) malloc(strlen(argv[1])+5+1);
    sprintf(filenames[2], "blue_%s", argv[1]);

    /* separate three color channels */
    bmp_t channels[3];
    for (int i = 0; i < 3; i++) {
        channels[i] = bmp_load(argv[1]);

        // delete the other two channels from each pixel
        for (int x = 0; x < channels[i].width; x++) {
            for (int y = 0; y < channels[i].height; y++) {
                color_t color = bmp_get_pixel(channels[i], x, y);
                color &= 0xFF0000 >> (i*8);
                bmp_set_pixel(channels[i], x, y, color);
            }
        }

        bmp_write(channels[i], filenames[i]);
    }

    /* free allocated bitmap memory */
    for (int i = 0; i < 3; i++)
        bmp_free(channels[i]);

    return 0;
}
