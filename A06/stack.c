#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Node *createNode(int line, int column, bracket_type type) {
    bracket_data brack_data = {line, column, type};

    Node *n = (Node *)malloc(sizeof(Node));

    if (n == NULL) {
        return NULL;
    }

    n->b_d = brack_data;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

void pushOntoStack(Node **head, Node **tail, int line, int column, bracket_type type) {
    Node *newNode = createNode(line, column, type);

    if (newNode == NULL) {
        printf("Unable to create new Node\n");
        return;
    }

    if (*head == NULL) {
        *head = newNode;
        *tail = newNode;
    } else if (*head == *tail) {
        *tail = newNode;
        (*head)->next = newNode;
        (*tail)->prev = *head;
    } else {
        // head and tail point to something and it's not the same Node
        (*tail)->next = newNode;
        newNode->prev = *tail;
        *tail = newNode;
    }

}

bracket_data popFromStack(Node **head, Node **tail) {

    if (*head == NULL) {
        // Nothing is on the stack
        return (bracket_data){-1, -1, OPENING_BRACKET};

    } 

    bracket_data b_data;
    
    if (*head == *tail) {
        // 1 item where head and tail both point to it
        b_data = (*head)->b_d;
        free(*head);
        *head = NULL;
        *tail = NULL;
    } else {
        // head and tail point to two different Nodes
        // remove tail Node
        b_data = (*tail)->b_d;
        Node *temp = *tail;
        *tail = (*tail)->prev;
        (*tail)->next = NULL;
        free(temp);
    }
    return b_data;
}

void clearStack(Node **head, Node **tail) {
    if (*head == NULL) {
        printf("Clearing stack that is empty...\n");
        return;
    }

    Node *temp;
    while (*head != *tail) {
        (*head)->prev = NULL;
        temp = *head;
        *head = (*head)->next;
        temp->next = NULL;
        free(temp);
    }

    (*head)->prev = NULL;
    free(*head);
    *head = NULL;
    *tail = NULL;
}

void printStack(Node **head) {
    Node *cursor = *head;

    while (cursor != NULL) {
        char br_type = cursor->b_d.type == OPENING_BRACKET ? '{' : '}';
        printf("(%d, %d) (%c) -> ", cursor->b_d.line, cursor->b_d.column, br_type);
        cursor = cursor->next;
    }
    printf("(NULL)\n");
}
