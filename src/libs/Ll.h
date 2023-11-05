#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *value;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *ptr;
    int size;
    void (*free)(void*);
} Llist;

Llist LlInit(void);
void LlAppend(Llist *ll, void *value);
int LlPop(Llist *ll, int index);
void *LlGetAt(Llist *ll, int index);
void LlFree(Llist *ll);
