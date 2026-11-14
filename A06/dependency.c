#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"

void printDependencies(char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        return;

    char *buffer = NULL;
    size_t len = 0;

    char *found;
    char *end;
    char buffer_arr[30];
    printf("%s has the following dependencies\n", filename);
    while (getline(&buffer, &len, fp) != EOF) {
        if (strstr(buffer, "#include") != NULL) {
            found = strstr(buffer, "<");
            char end_char;
            if (found != NULL) {
                end_char = '>';
            } else {
                found = strstr(buffer, "\"");
                if (found == NULL)
                    continue;
                end_char = '"';
            }
            int length = 0;
            while (*(found + 1 + length) != end_char && *(found + 1 + length) != '\0' 
                    && length < 29) {
                buffer_arr[length] = *(found + 1 + length);
                length++;
            }
            buffer_arr[length] = '\0';
            printf("\t%s\n", buffer_arr);
        }
    }

    free(buffer);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: Must include at least one file './dependency animal.h'\n");
        return 1;
    }

    struct tree_node *root = NULL;

    // Process files passed in through command-line
    for (int i = 1; i < argc; i++) {
        root = insert(argv[i], root);
    }


    // go into infinite loop
    char input[250];
    while (printf("$ ") && scanf("%249s", input) == 1) {
        if (strcmp(input, "list") == 0) {
            printSorted(root);
        } else if (strcmp(input, "quit") == 0) {
            break;
        } else {
            // Check if file given exists
            struct tree_node *node = find(input, root);
            if (node == NULL) {
                printf("%s not found\n", input);
                continue; // we can't process a non-existent file
            }
            // get the dependencies of the file
            printDependencies(input); 
        }
    }

    clear(root);
}
