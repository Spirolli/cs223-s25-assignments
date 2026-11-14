/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  int w, h;

  struct ppm_pixel* pix = read_ppm(argv[1], &w, &h);
  if (pix == NULL) {
      printf("Error: Could not read file %s\n", argv[1]);
      return 1;
  }

  int max_chars = (w * 3 * h) / 8;
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", max_chars);
  
  int index, byte_index = 0;
  unsigned char byte_char = 0;
  char pixel_byte;

  for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
          index = w * i + j;
          for (int k = 0; k < 3; k++) {
              switch(k) {
                  case 0:
                      pixel_byte = pix[index].red;
                      break;
                  case 1:
                      pixel_byte = pix[index].green;
                      break;
                  case 2:
                      pixel_byte = pix[index].blue;
                      break;
              }
              byte_char = (byte_char << 1) | (pixel_byte & 1);
              byte_index++;

              if (byte_index == 8) {
                  if (byte_char == '\0') {
                      printf("\n");
                      free(pix);
                      return 0;
                  }
                  printf("%c", byte_char);
                  byte_char = 0;
                  byte_index = 0;
              }
          }
      }
  }
  printf("\n");
  free(pix);
  return 0;
}

