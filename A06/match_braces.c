#include <stdio.h>

#include "stack.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("usage: Must provide file to read from\n");
        return 1;
    }

    FILE *fp = fopen(argv[1],"r");

    if (fp == NULL) {
        printf("File '%s' not found. Exiting...\n", argv[1]);
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL;

    int column_number = 0;
    int line_number = 1;

    char c;
    bracket_data br_d;
    while ( (c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            line_number += 1;
            column_number = 1;
            continue;
        } else if (c == '{') {
            pushOntoStack(&head, &tail, line_number, column_number, 
                    OPENING_BRACKET);
        } else if (c == '}') {
            br_d = popFromStack(&head, &tail);
            if (br_d.line == -1) {
                printf("Unmatched brace on Line %d and Column %d\n",
                        line_number, column_number);
            } else {
                printf("Found matching braces: (%d, %d) -> (%d, %d)\n",
                        br_d.line, br_d.column,
                        line_number, column_number);
            }
        }
        column_number++;
    }

    while (head != NULL) {
        // There is still something left on the stack
        br_d = popFromStack(&head, &tail);
        printf("Unmatched brace on Line %d and Column %d\n",
                br_d.line, br_d.column);
    }

    clearStack(&head, &tail);

    return 0;
}

