/*   main.c   */

#include <stdio.h>
#include <bitmap.h>

int main(int argc, char *argv[]) {
    bmp_t bmp = bmp_create(100, 100);

    // for (int i = 0; i < bmp.width; i++)
    //     for (int j = 0; j < bmp.height/2; j++)
    //         bmp_set_pixel(bmp, i, j, 0xFFFFFF);

    bmp_set_pixel(bmp, 0, 0, 0x000000);

    for (int i = 0; i < bmp.width; i++)
        for (int j = 0; j < bmp.height/2; j++)
            bmp_set_pixel(bmp, i, j, 0xFFFFFF);

    for (int i = 0; i < bmp.width; i++)
        for (int j = bmp.height/2; j < bmp.height; j++)
            bmp_set_pixel(bmp, i, j, 0xFF0000);

    bmp_set_pixel(bmp, 0, 0, 0x000000);

    bmp_write(bmp, "file.bmp");

    // bmp_free(bmp);
	return 0;
}
