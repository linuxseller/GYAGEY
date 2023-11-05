#include "libs/Ll.h"
#include "dataTypes.h"
#include "free.h"
#include "gui.h"
#include "libs/clogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* key;
    char *value;
} JsonPairString;

#define ENTRYCHECK(str){ \
    if(strcmp(jsonEntry, str)!=0){ \
        ShowErrorFrontend("[ERROR] Scene format error.", "Game file is broken. Try reinstalling game files."); \
        clogger(CLOGGER_ERROR, "scene json format error in %s: got %s\n", str, jsonEntry); \
        exit(1); \
    } \
}

char *JsonReadWholeFile(FILE *f);
char *JsonPEAS(char **jsonText);
int JsonHasNextEntry(char *jsonText);
Llist *JsonParseStringArrToLl(char **jsonText);
Llist *ParseChoicesJson(char **gameJson);
Scene *ParseSceneJson(char **gameJson);
