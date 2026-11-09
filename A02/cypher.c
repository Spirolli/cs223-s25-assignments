/***************************************************
 * Author: 
 * 
 */
#include <stdio.h>
#include <string.h>

int main() {
    char s[30];
    int shift;

    printf("Enter a word: ");
    scanf("%s", s);
    printf("Enter a shift: ");
    scanf("%d", &shift);
    printf("The shift is %d\n", shift);

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] + shift < 'a') {
            s[i] += (26 + shift);
        } else if (s[i] + shift > 'z') {
            s[i] -= (26 - shift);
        } else {
            s[i] += shift;
        }
    }

    printf("Your cypher is %s\n", s);
    return 0;
}
