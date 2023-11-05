#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Ll.h"

typedef struct {
    char *key;
    void *value;
} AssPair;

typedef Llist AssList;
AssList AssInit(void);
void AssAppend(AssList *list, char *key, void *value);
void* AssGet(AssList *list, char *key);
