/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char *argv[]) {

    srand(time(NULL));

    if (argc < 2) {
        printf("Must include .ppm file to change\n");
        return 1;
    }

    char s[100];
    int base_len = strlen(argv[1]) - 4;

    strncpy(s, argv[1], base_len);
    s[base_len] = '\0';
    strcat(s, "-glitch.ppm");

    printf("Filename: '%s'\n",s);

    int w, h;
    struct ppm_pixel *pixels = read_ppm(argv[1], &w, &h);
    if (pixels == NULL) {
        printf("Error reading file\n");
        return 1;
    }

    FILE *fp = fopen(s, "wb");
    if (fp == NULL) {
        printf("File not created");
        return 1;
    }
    printf("Writing file %s\n", s);

    int index;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            index = w * i + j;
            pixels[index].red = pixels[index].red << (rand() % 2);
            pixels[index].green = pixels[index].green << (rand() % 2);
            pixels[index].blue = pixels[index].blue << (rand() % 2);
        }
    }

    write_ppm(s, pixels, w, h);
    free(pixels);
    return 0;
}
