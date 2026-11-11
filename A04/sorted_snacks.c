/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[40];
    float cost;
    int quantity;
} snack;

struct Node {
    snack s;
    struct Node *next;
};

typedef int (*CompareFunc)(const struct Node*, const struct Node *);

struct Node *insert_first (struct Node *head, struct Node *n);
struct Node *createList(int num_snacks);
void menu(struct Node *head);
void clear();
void printList(struct Node *head);
void splitList(struct Node *source, struct Node **frontRef, struct Node **backRef);
struct Node *sortedMerge(struct Node *a, struct Node *b, CompareFunc cmp);
struct Node *mergeSort(struct Node *head, CompareFunc cmp); 


// Sorting comparison functions ----------------

int sortCost(const struct Node *a, const struct Node *b) {
    if (a->s.cost < b->s.cost)
        return -1;
    return a->s.cost > b->s.cost ? 1 : 0;
}

int sortName(const struct Node *a, const struct Node *b) {
    return strcmp(a->s.name, b->s.name);
}

int sortQuantity(const struct Node *a, const struct Node *b) {
    return a->s.quantity - b->s.quantity;
}

void splitList(struct Node *source, struct Node **frontRef, struct Node **backRef) {
    struct Node *fast;
    struct Node *slow;
    slow = source;
    fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef =slow->next;
    slow->next = NULL;
}

struct Node *sortedMerge(struct Node *a, struct Node *b, CompareFunc cmp) {
    struct Node *result = NULL;

    // base cases
    if (a == NULL) return b;
    if (b == NULL) return a;

    if (cmp(a, b) <= 0) {
        result = a;
        result->next = sortedMerge(a->next, b, cmp);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next, cmp);
    }
    return result;
}

struct Node *mergeSort(struct Node *head, CompareFunc cmp) {

    if (head == NULL || head->next == NULL) {
        return head;
    }

    struct Node *a;
    struct Node *b;

    splitList(head, &a, &b);

    a = mergeSort(a, cmp);
    b = mergeSort(b, cmp);

    return sortedMerge(a, b, cmp);
}


int main() {
    int num_snacks;

    printf("Enter a number of snacks: ");
    scanf("%d", &num_snacks);

    struct Node *head = createList(num_snacks);

    menu(head);

    clear(head);

    return 0;
}

struct Node *insert_first (struct Node *head, struct Node *n) {
    // Creates a new snack struct and insert at beginning
    // of linked list
    n->next = head;
    return n;
}

struct Node *createList(int num_snacks) {
    struct Node *head = NULL;

    char name[40];
    float cost;
    int quantity;

    for (int i = 0; i < num_snacks; i++) {
        printf("Enter a name: ");
        scanf("%s", name);
        printf("Enter a cost: ");
        scanf("%f", &cost);
        printf("Enter a quantity: ");
        scanf("%d", &quantity);

        struct Node *n = (struct Node *)malloc(sizeof(struct Node));
        if (n == NULL) {
            printf("Memory allocation failed!\n");
            return head;
        }
        strncpy(n->s.name, name, sizeof(n->s.name) - 1);
        n->s.name[sizeof(n->s.name) - 1] = '\0';
        n->s.cost = cost;
        n->s.quantity = quantity;
        head = insert_first(head, n);
    }
    
    return head;
}

void printOptions() {
    printf("\n\tOptions:\n\t-----------------\n");
    printf("\t1) sort by Name\n");
    printf("\t2) sort by Cost\n");
    printf("\t3) sort by Quantity\n");
    printf("\t4) Quit\n");
}

void menu(struct Node *head) {
    int option = 0;

    do {
        printf("\n-----------------------------------\n");
        printf("Welcome to Sorted Sally's Snack Bar.\n");
        printf("-----------------------------------\n");

        printList(head);

        printOptions();

        printf("\nWhat would you like to do? ");

        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("...Sorting By Name\n");
                head = mergeSort(head, sortName);
                break;
            case 2:
                printf("...Sorting By Cost\n");
                head = mergeSort(head, sortCost);
                break;
            case 3:
                printf("...Sorting By Quantity\n");
                head = mergeSort(head, sortQuantity);
                break;
            case 4:
                printf("...Exiting\n");
                break;
            default:
                printf("Option not recognized...Exiting\n");
                option = 4;
                break;
        }
    } while (option != 4);
}


void clear(struct Node *head) {
    // Clear and free items in list
    
    struct Node *cursor;
    while (head != NULL) {
        cursor = head->next;
        free(head);
        head = cursor;
    }
}

void printList(struct Node *head) {
    // Prints the content of the list
    int index = 0;
    int min_width = 15;
    struct Node *cursor = head;
    while (cursor != NULL) {
        printf("%d %-*scost: $%.2f\tquantity: %d\n",
                index++, min_width, cursor->s.name,
                cursor->s.cost, cursor->s.quantity);
        cursor = cursor->next;
    }
}
