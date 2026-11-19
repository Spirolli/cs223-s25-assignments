#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include "read_ppm.h"
#include "write_ppm.h"

#define BRIGHTNESS_THRESHOLD 200
#define BLUR_BOX_SIZE 25
#define COLOR_CHANNELS 3

int is_out_of_bounds(int row, int column, int width, int height) {
    if ( (row < 0) || (row >= height) || (column < 0) || (column >= width) ) {
        return 1;
    }
    return 0; // returns false because we are NOT out of bounds
}

void color_box_blur(struct ppm_pixel *src, struct ppm_pixel *dst,
        int index, int row, int col, int w, int h) {
    int dimension = (int) sqrt(BLUR_BOX_SIZE);
    int half = dimension / 2;

    int start_row = row - half;
    int end_row = row + half;
    int start_col = col - half;
    int end_col = col + half;

    int actual_dimension = 0; // For clamped blur boxes
    unsigned int r_val = 0, g_val = 0, b_val = 0;

    for (int i = start_row; i <= end_row; i++) {
        for (int j = start_col; j <= end_col; j++) {
            if ( is_out_of_bounds(i, j, w, h) ) {
                // blur box index or cursor points to value outside of image
                // move on
                continue;
            }
            int neighbor_index = i * w + j;
            actual_dimension++;
            r_val += src[neighbor_index].red;
            g_val += src[neighbor_index].green;
            b_val += src[neighbor_index].blue;
        }
    }
    dst[index].red = r_val / actual_dimension;
    dst[index].green = g_val / actual_dimension;
    dst[index].blue = b_val / actual_dimension;
}

int get_brightness(struct ppm_pixel pixel) {
    return (pixel.red + pixel.green + pixel.blue) / 3;
}

unsigned char clamp(int val) {
    return (val > 255) ? 255: (unsigned char)val;
}

void apply_glow(struct ppm_pixel *pixels, int w, int h) {

    int total_pixels = w * h;
    struct ppm_pixel *bright_pixels = calloc(total_pixels, sizeof(struct ppm_pixel));
    struct ppm_pixel *blurred_glow = malloc(sizeof(struct ppm_pixel) * total_pixels);

    for (int i = 0; i < total_pixels; i++) {
        if (get_brightness(pixels[i]) > BRIGHTNESS_THRESHOLD) {
            // It's bright enough to warrant 'blurring'
            bright_pixels[i] = pixels[i];
        }
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int index = i * w + j;
            color_box_blur(bright_pixels, blurred_glow, index, i, j, w, h);

            pixels[index].red = clamp(pixels[index].red + blurred_glow[index].red);
            pixels[index].green = clamp(pixels[index].green + blurred_glow[index].green);
            pixels[index].blue = clamp(pixels[index].blue + blurred_glow[index].blue);
        }
    }
    // Free the copy pixels when we don't need it anymore
    free(bright_pixels);
    free(blurred_glow);
}

int main(int argc, char* argv[]) {

    int w, h;
    struct ppm_pixel *pixels = read_ppm("earth-small.ppm", &w, &h);

    printf("Successfully read %s with width %d and height %d\n", "earth-small.ppm", w, h);


    // compute image
    apply_glow(pixels, w, h);
    
    // write image to ppm_file
    write_ppm("glow.ppm", pixels, w, h);

    // free pixel since we don't need it anymore
    free(pixels);
    return 0;
}
