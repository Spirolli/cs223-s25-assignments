#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

struct tree_node* find(const char* name, struct tree_node* root)
{
    struct tree_node *cursor = root;

    int comp = strcmp(name, cursor->data.name);

    while (comp != 0) {
        if (comp < 0) {
            if (cursor->left == NULL) {
                break;
            }
            cursor = cursor->left;
        } else {
            if (cursor->right == NULL) {
                break;
            }
            cursor = cursor->right;
        }
        comp = strcmp(name, cursor->data.name);
    }

    if (comp == 0) {
        // The node was found
        return cursor;
    } else {
        // The node was not found
        return NULL;
    }
}

struct tree_node* insert(const char* name, struct tree_node* root)
{
    struct tree_node *newNode = malloc(sizeof(struct tree_node));
    strcpy(newNode->data.name, name);
    newNode->left = NULL;
    newNode->right = NULL;

    if (root == NULL) {
        root = newNode;
        return root;
    }

    struct tree_node *cursor = root;

    int cmp = strcmp(name, cursor->data.name);

    while (1) {
        if (cmp < 0) {
            if (cursor->left == NULL) {
                cursor->left = newNode;
                break;
            }
            cursor = cursor->left;
        } else {
            if (cursor->right == NULL) {
                cursor->right = newNode;
                break;
            }
            cursor = cursor->right;
        }
        cmp = strcmp(name, cursor->data.name);
    }

    return root;
}

void clear(struct tree_node* root)
{
    if (root == NULL) {
        return;
    }

    if ( root->left != NULL ) {
        clear(root->left);
    }

    if ( root->right != NULL ) {
        clear(root->right);
    }

    free(root);
}

void print(struct tree_node* root)
{
    printWithSpaces(root, 0);
}

void printWithSpaces(struct tree_node* root, int num_spaces) 
{

    if (root == NULL) {
        printf("()\n");
    }

    printf("%s\n", root->data.name);
    
    if (root->left != NULL) {
        printf("%*sl:", num_spaces + 1, "");
        printWithSpaces(root->left, num_spaces + 1);
    }

    if (root->right != NULL) {
        printf("%*sr:", num_spaces + 1, "");
        printWithSpaces(root->right, num_spaces + 1);
    }
}

void printSorted(struct tree_node* root)
{
    if (root == NULL) {
        return;
    }

    if (root->left != NULL) {
        printSorted(root->left);
    }

    printf("%s\n", root->data.name);

    if (root->right != NULL) {
        printSorted(root->right);
    }
}

