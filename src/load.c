/*   load.c   */

#include <stdio.h>
#include <bitmap.h>

int main(int argc, char *argv[]) {
    bmp_t bmp = bmp_load("file.bmp");
    // bmp_set_pixel(bmp, 1, 1, 0xFF0000);
    // bmp_write(bmp, "file2.bmp");
    // printf("%d %d\n", bmp.width, bmp.height);
    // bmp_free(bmp);
	return 0;
}
