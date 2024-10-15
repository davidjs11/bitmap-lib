/*   main.c   */

#include <stdio.h>
#include <bitmap.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    bmp_t bmp = bmp_create(40, 40);

    /* draw poland flag */
    for (int i = 0; i < bmp.width; i++)
        for (int j = 0; j < bmp.height/2; j++)
            bmp_set_pixel(bmp, i, j, RGB(255, 255, 255));

    for (int i = 0; i < bmp.width; i++)
        for (int j = bmp.height/2; j < bmp.height; j++)
            bmp_set_pixel(bmp, i, j, RGB(255, 255, 0));

    bmp_write(bmp, "file.bmp");
    bmp_free(bmp);
	return 0;
}
