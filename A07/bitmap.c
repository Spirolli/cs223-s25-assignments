/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // todo: your code here
  int col = 0;
  for (int i = 0; i < 64; i++) {
      if (img & (0x1ul << (64 - i - 1))) {
          printf("@");
      } else {
          printf(" ");
      }
      col++;
      if (col == 8) {
          col = 0;
          printf("\n");
      }
  }
  return 0;
}
