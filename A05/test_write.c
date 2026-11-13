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

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  // test writing the file to test.ppm, reload it, and print the contents
  write_ppm("test.ppm", pixels, w, h);

  int test_w, test_h;
  struct ppm_pixel* test_pixels = read_ppm("test.ppm", &test_w, &test_h);
  printf("Testing file feep-raw.ppm: %d %d\n", test_w, test_h);
  int test_index;
  for (int i = 0; i < test_h; i++) {
      for (int j = 0; j < test_w; j++) {
          test_index = test_h * i + j;
          printf("(%u,%u,%u) ", test_pixels[test_index].red, test_pixels[test_index].green,
                  test_pixels[test_index].blue);
      }
      printf("\n");
  }
  free(pixels);
  free(test_pixels);
  return 0;
}
