/***************************************************
 * Author: 
 * 
 */
#include <stdio.h>

struct snack {
    char *name;
    float cost;
    int quantity;
};

#define TOTAL_SNACKS 3

int main() {

    int min_width = 15;

    struct snack snacks[TOTAL_SNACKS] = {
        {"Coco Puffs", 1.50, 4},
        {"Manchego cheese", 15.50, 6},
        {"Magic beans", 0.50, 0}
    };

    float money;

    // INTRODUCTION
    printf("Welcome to Steven Struct's Snack Bar.\n");
    // ====GET MONEY AMOUNT
    printf("How much money do you have? ");
    scanf("%f", &money);

    // ITEM PRINTOUT
    printf("\nItems\n-------------------\n");
    for (int i = 0; i < TOTAL_SNACKS; i++) {
        printf("%d) %-*s\tcost: $%.2f\tquantity: %d\n",
                i, min_width, snacks[i].name, snacks[i].cost, snacks[i].quantity);
    }

    printf("\nWhat snack would you like to buy? [");
    for (int i = 0; i < TOTAL_SNACKS; i++) {
        if (i > 0)
            printf(",");
        printf("%d", i);
    }
    printf("] ");

    int selection;

    scanf("%d", &selection);

    if ((selection < 0) || (selection >= TOTAL_SNACKS)) {
        printf("Make a choice out of the selection\n");
        return 1;
    }

    if ( snacks[selection].cost > money ) {
        printf("You can't afford it!\n");
    } else if ( snacks[selection].quantity == 0) {
        printf("Sorry we are out of %s\n", snacks[selection].name);
    } else {
        printf("You bought %s\n", snacks[selection].name);
        money -= snacks[selection].cost;
        snacks[selection].quantity--;
        printf("There are now %d %s\n", snacks[selection].quantity, snacks[selection].name);
        printf("You have %.2f\n", money);
    }

    return 0;
}
