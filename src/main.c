/*   main.c   */

#include <stdio.h>
#include <bitmap.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    bmp_t bmp = bmp_create(40, 40);

    /* draw 3 RGB colors */
    for (int i = 0; i < bmp.width/3; i++)
        for (int j = 0; j < bmp.height; j++)
            bmp_set_pixel(bmp, i, j, RGB(255, 0, 0));

    for (int i = bmp.width/3; i < 2*bmp.width/3; i++)
        for (int j = 0; j < bmp.height; j++)
            bmp_set_pixel(bmp, i, j, RGB(0, 255, 0));

    for (int i = 2*bmp.width/3; i < bmp.width; i++)
        for (int j = 0; j < bmp.height; j++)
            bmp_set_pixel(bmp, i, j, RGB(0, 0, 255));

    bmp_write(bmp, "file.bmp");
    bmp_free(bmp);
	return 0;
}
