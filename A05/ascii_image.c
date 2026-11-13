/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include "read_ppm.h"
#include "write_ppm.h"

char get_intensity_char(struct ppm_pixel pixel) {
    int intensity = pixel.red + pixel.green + pixel.blue; 
    intensity /= 3;
    if (intensity <= 25) {
        return '@';
    } else if (intensity <= 50) {
        return '#';
    } else if (intensity <= 75) {
        return '%';
    } else if (intensity <= 100) {
        return '*';
    } else if (intensity <= 125) {
        return 'o';
    } else if (intensity <= 150) {
        return ';';
    } else if (intensity <= 175) {
        return ':';
    } else if (intensity <= 200) {
        return ',';
    } else if (intensity <= 225) {
        return '.';
    } 
    return ' ';
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Need to file to read from\n");
        return 1;
    }

    int w, h;
    struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
    if (pixels == NULL) {
        return 1;
    }
    printf("Reading %s with width %d and height %d\n", argv[1], w, h);


    int index;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            index = i * w + j;
            printf("%c", get_intensity_char(pixels[index]));
        }
        printf("\n");
    }
}
