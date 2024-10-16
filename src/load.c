/*   load.c   */

#include <stdio.h>
#include <bitmap.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    bmp_t bmp = bmp_load("file.bmp");
    bmp_set_pixel(bmp, 1, 1, 0x000000);
    bmp_write(bmp, "file2.bmp");
    bmp_free(bmp);
    return 0;
}
