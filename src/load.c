/*   load.c   */

#include <stdio.h>
#include <bitmap.h>

int main(int argc, char *argv[]) {
    bmp_t bmp = bmp_load("file.bmp");
    bmp_set_pixel(bmp, 1, 1, 0xFF0000);
    bmp_write(bmp, "file2.bmp");
    printf("0x%06X", bmp_get_pixel(bmp, 0, 0));
    // bmp_free(bmp);
	return 0;
}
