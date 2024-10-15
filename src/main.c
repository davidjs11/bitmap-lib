/*   main.c   */

#include <stdio.h>
#include <bitmap.h>

int main(int argc, char *argv[]) {
    bmp_t bmp = bmp_create(40, 40);

    for (int i = 0; i < bmp.width; i++)
        for (int j = 0; j < bmp.height/2; j++)
            bmp_set_pixel(bmp, i, j, 0xFFFFFF);

    for (int i = 0; i < bmp.width; i++)
        for (int j = bmp.height/2; j < bmp.height; j++)
            bmp_set_pixel(bmp, i, j, 0xFF0000);

    for (int i = 0; i < bmp.width; i++) {
        bmp_set_pixel(bmp, i, i, 0x000000);
        bmp_set_pixel(bmp, i+1, i, 0x000000);
        bmp_set_pixel(bmp, i+2, i, 0x000000);
    }

    bmp_write(bmp, "file.bmp");

    // bmp_free(bmp);
	return 0;
}
