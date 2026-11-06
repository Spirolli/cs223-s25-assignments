/***************************************************
 * mathgame.c
 * Author: 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN 1
#define MAX 20

int get_random_int() {
    return (rand() % (MAX - MIN + 1)) + MIN;
}

int main() {
    srand(time(NULL));

    int num_rounds;
    printf("Welcome to Math Game!\n");
    printf("How many rounds do you want to play? ");
    scanf("%d", &num_rounds);
    printf("Rounds to play %d\n", num_rounds);

    int correctly_answered = 0;
    int rand_a, rand_b, answer;
    for (int i = 0; i < num_rounds; i++) {
        printf("\n");
        rand_a = get_random_int();
        rand_b = get_random_int();
        printf("%d + %d = ? ", rand_a, rand_b);
        scanf("%d", &answer);
        if (answer == (rand_a + rand_b)) {
            printf("Correct!\n");
            correctly_answered++;
        }
    }
    printf("You answered %d/%d correctly.\n", correctly_answered, num_rounds);
    return 0;
}
