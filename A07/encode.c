/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

void change_pixels(struct ppm_pixel *pix, char enc_phrase[], int w, int h) {
  int enc_phrase_index = 0;
  unsigned char byte_char = enc_phrase[enc_phrase_index++];
  unsigned char *current_pixel = NULL;

  int bit_index = 0;
  int total_bits = (strlen(enc_phrase) + 1) * 8;

  for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
          int index = i * w + j;
          for (int pixel_index = 0; pixel_index < 3; pixel_index++) {
              if (pixel_index == 0) current_pixel = &pix[index].red;
              else if (pixel_index == 1) current_pixel = &pix[index].green;
              else if (pixel_index == 2) current_pixel = &pix[index].blue;

              unsigned char bit = (byte_char >> 7) & 1;

              *current_pixel = (*current_pixel & 0xFE) | bit;

              byte_char = byte_char << 1;
              bit_index++;
              if (bit_index == total_bits) {
                  return;
              }

              if ( bit_index % 8 == 0 ) {
                  byte_char = enc_phrase[enc_phrase_index++];
              }
          }
      }
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  int w, h;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
  int max_chars = (w * 3 * h) / 8;

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", max_chars);
  
  char encode_phrase[256];

  printf("Enter a phrase: ");
  if (fgets(encode_phrase, sizeof(encode_phrase), stdin) == NULL) {
      free(pixels);
      return 1;
  }

  // Strip trailing newline added by fgets
  encode_phrase[strcspn(encode_phrase, "\n")] = '\0';

  if (strlen(encode_phrase) + 1 > max_chars) {
      printf("Phrase to encode is longer than the file\n");
      free(pixels);
      return 1;
  }

  // Creating new filename
  char new_filename[64];
  int i = 0;
  char *dot = strchr(argv[1], '.');
  while (dot != NULL && argv[1] + i < dot) {
      new_filename[i] = argv[1][i];
      i++;
  }
  new_filename[i] = '\0';
  strcat(new_filename, "-encoded.ppm");
  printf("Writing file %s\n", new_filename);

  change_pixels(pixels, encode_phrase, w, h);

  write_ppm(new_filename, pixels, w, h);

  free(pixels);

  return 0;
}

