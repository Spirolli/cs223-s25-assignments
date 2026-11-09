/***************************************************
 * Author: 
 * 
 */
#include <stdio.h>
#include <string.h>

int main() {
    char s[20];
    printf("Enter a word: ");
    scanf("%s", s);
    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == 'e')
            s[i] = '3';
        else if (s[i] == 'l')
            s[i] = '1';
        else if (s[i] == 'a')
            s[i] = '@';
    }
    printf("Your bad password is %s\n", s);
    return 0;
}
