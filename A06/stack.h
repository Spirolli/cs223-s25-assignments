#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    OPENING_BRACKET,
    CLOSING_BRACKET
} bracket_type;

typedef struct {
    int line;
    int column;
    bracket_type type;
} bracket_data;

typedef struct Node Node;

struct Node {
    bracket_data b_d;
    Node *next;
    Node *prev;
};

Node *createNode(int line, int column, bracket_type type);

void pushOntoStack(Node **head, Node **tail, int line, int column, bracket_type type);

bracket_data popFromStack(Node **head, Node **tail);

void clearStack(Node **head, Node **tail);
void printStack(Node **head);

#endif
