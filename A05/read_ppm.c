/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "read_ppm.h"

typedef struct {
    int width;
    int height;
    int maxval;
    struct ppm_pixel *data;
} PPMImage;

void skip_comments(FILE *fp) {
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (isspace(ch)) { 
            continue;
        } else if (ch == '#') {
            while ((ch = fgetc(fp)) != EOF && ch != '\n');
        } else {
            ungetc(ch, fp);
            break;
        }
    }
}

PPMImage *read_ppm_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    char magic[3];
    if (!fgets(magic, sizeof(magic), fp) || magic[0] != 'P' || magic[1] != '6') {
        fprintf(stderr, "Invalid PPM format (expected P6)\n");
        fclose(fp);
        return NULL;
    }

    PPMImage *img = malloc(sizeof(PPMImage));
    if (!img) {
        fclose(fp);
        return NULL;
    }

    skip_comments(fp);
    if(fscanf(fp, "%d", &img->width) != 1) goto error;
    skip_comments(fp);
    if(fscanf(fp, "%d", &img->height) != 1) goto error;
    skip_comments(fp);
    if(fscanf(fp, "%d", &img->maxval) != 1) goto error;

    if (img->maxval > 255) {
        fprintf(stderr, "16-bit PPM files are not supported in this example");
        goto error;
    }

    fgetc(fp);

    size_t num_pixels = (size_t)img->width * img->height;
    img->data = malloc(num_pixels * sizeof(struct ppm_pixel));
    if (!img->data) goto error;

    if (fread(img->data, sizeof(struct ppm_pixel), num_pixels, fp) != num_pixels) {
        fprintf(stderr, "Unexpected end of file while reading pixels\n");
        free(img->data);
        goto error;
    }

    fclose(fp);
    return img;

error:
    free(img);
    fclose(fp);
    return NULL;
}

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
    PPMImage *img = read_ppm_file(filename);
    if (!img) return NULL;

    *w = img->width;
    *h = img->height;

    return img->data;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
    return NULL;
}
