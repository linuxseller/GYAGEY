#pragma  once
#include <raylib.h>
#include "libs/Ll.h"

typedef struct {
    char *name;
    char *line;
} VoiceLine;

typedef struct {
    int *id;
    char *text;
} Choice;

// Next variable: -1 -> scene is last scene
//                 0 -> end of list
typedef struct {
    int number;
    Llist *characterLl;
    Texture2D *background;
    Llist *voiceLineLl;
    Music *music;
    Llist *choicesLl;
    int nextSceneId;
} Scene;

