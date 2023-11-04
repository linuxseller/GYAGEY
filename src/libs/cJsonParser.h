#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Ll.h"

typedef struct {
    char* key;
    char *value;
} JsonPairString;


char *JsonReadWholeFile(FILE *f);
char *JsonPEAS(char **jsonText);
int JsonHasNextEntry(char *jsonText);
Llist *JsonParseStringArrToLl(char **jsonText);
