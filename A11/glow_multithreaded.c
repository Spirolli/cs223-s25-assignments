#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct t_arg {
    struct ppm_pixel *pixels;
    struct ppm_pixel *bright_pix; // pointer to shared file array
    struct ppm_pixel *blurred_glow; // pointer to shared file array
    int w;
    int h;
    int start_row;
    int end_row;
    int blursize;
};

int is_out_of_bounds(int row, int column, int width, int height) {
    return ( (row < 0) || (row >= height) || (column < 0) || (column >= width) );
}

unsigned char clamp(int val) {
    return (val > 255) ? 255: (unsigned char)val;
}

void *blur_and_blend_worker(void *args) {
    struct t_arg * myargs = (struct t_arg *) args;

    struct ppm_pixel *pixels = myargs->pixels;
    struct ppm_pixel *src = myargs->bright_pix;
    struct ppm_pixel *dst = myargs->blurred_glow;
    int w = myargs->w;
    int h = myargs->h;
    int half = myargs->blursize / 2;

    for (int row = myargs->start_row; row < myargs->end_row; row++) {
        for (int col = 0; col < w; col++) {
            int index = row * w + col;

            int actual_dimension = 0;
            unsigned int r_val = 0, g_val = 0, b_val = 0;

            // Box blur around (row, col)
            for (int i = row - half; i <= row + half; i++) {
                for (int j = col - half; j <= col + half; j++) {
                    if (is_out_of_bounds(i, j, w, h)) {
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

            // Blend directly into this thread's assigned pixels
            pixels[index].red = clamp(pixels[index].red + dst[index].red);
            pixels[index].green = clamp(pixels[index].green + dst[index].green);
            pixels[index].blue = clamp(pixels[index].blue + dst[index].blue);
        }
    }
    return NULL;
}

int get_brightness(struct ppm_pixel pixel) {
    return (pixel.red + pixel.green + pixel.blue) / 3;
}


void apply_glow(struct ppm_pixel *pixels, int w, int h, int num_threads,
        int threshold, int blursize) {

    int total_pixels = w * h;
    struct ppm_pixel *bright_pixels = calloc(total_pixels, sizeof(struct ppm_pixel));
    struct ppm_pixel *blurred_glow = malloc(sizeof(struct ppm_pixel) * total_pixels);

    for (int i = 0; i < total_pixels; i++) {
        if (get_brightness(pixels[i]) > threshold) {
            // It's bright enough to warrant 'blurring'
            bright_pixels[i] = pixels[i];
        }
    }

    pthread_t *thread_array = malloc(num_threads * sizeof(pthread_t));
    struct t_arg *thread_args = malloc(num_threads * sizeof(struct t_arg));

    int rows_per_thread =  h / num_threads;

    // Populate thread arguments for all the threads.
    for (int i = 0; i < num_threads; i++) {
        thread_args[i].pixels = pixels;
        thread_args[i].bright_pix = bright_pixels;
        thread_args[i].blurred_glow = blurred_glow;
        thread_args[i].w = w;
        thread_args[i].h = h;
        thread_args[i].blursize = blursize;
        thread_args[i].start_row = i * rows_per_thread;
        thread_args[i].end_row = (i == num_threads - 1) ? h : (i + 1) * rows_per_thread;
        pthread_create(&thread_array[i], NULL, blur_and_blend_worker, &thread_args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(thread_array[i], NULL);
    }

    // Free the pixel buffers that are not needed for computation
    free(bright_pixels);
    free(blurred_glow);
    // Free the buffers for the threads
    free(thread_array);
    free(thread_args);
}


int main(int argc, char* argv[]) 
{
    int N = 4;
    int threshold = 200;
    int blursize = 24;
    const char* filename = "earth-small.ppm";

    int opt;
    while ((opt = getopt(argc, argv, ":N:t:b:f:")) != -1) {
        switch (opt) {
            case 'N': N = atoi(optarg); break;
            case 't': threshold = atof(optarg); break;
            case 'b': blursize = atof(optarg); break;
            case 'f': filename = optarg; break;
            case '?': printf("usage: %s -N <NumThreads> -t <brightness threshold> -b <box blur size> -f <ppmfile>\n", argv[0]); break;
        }
    }
    int w, h;
    struct ppm_pixel *pixels = read_ppm(filename, &w, &h);

    printf("Successfully read %s with width %d and height %d\n", "earth-small.ppm", w, h);

    // compute image
    apply_glow(pixels, w, h, N, threshold, blursize);

    // write image to ppm_file
    write_ppm("glow_mt.ppm", pixels, w, h);

    free(pixels);
    return 0;
}
