/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return;
    }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d 255\n", w, h);
    int index;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            index = w * i + j;
            fprintf(f, "%c%c%c", pixels[index].red, pixels[index].green, pixels[index].blue);
        }
    }
    fclose(f);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
