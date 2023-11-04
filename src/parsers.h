#include "libs/Ll.h"
#include "libs/cJsonParser.h"
#include "dataTypes.h"
#include "free.h"
#include "gui.h"
#include "libs/clogger.h"
#define ENTRYCHECK(str){ \
    if(strcmp(jsonEntry, str)!=0){ \
        ShowErrorFrontend("[ERROR] Scene format error.", "Game file is broken. Try reinstalling game files."); \
        clogger(CLOGGER_ERROR, "scene json format error in %s: got %s\n", str, jsonEntry); \
        exit(1); \
    } \
}

Llist *ParseChoicesJson(char **gameJson);

Scene *ParseSceneJson(char **gameJson);
