#include "parsers.h"

char *JsonReadWholeFile(FILE *f){
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    return string;
}

char *JsonPEAS(char **jsonText){
    char *key = calloc(100, 1);
    while(**jsonText != '"'){
        *jsonText += 1;
    }
    *jsonText+=1;
    for(int i=0; **jsonText!='"'; i++){
        if(**jsonText=='\\'){
            *jsonText+=1;
        }
        key[i]=**jsonText;
        *jsonText+=1;
    }
    *jsonText+=1;
    return key;
}

int JsonHasNextEntry(char *jsonText){
    for(int i=0; jsonText[i]!=0; i++){
        if(jsonText[i]=='"'){return 1;}
    }
    return 0;
}

Llist *JsonParseStringArrToLl(char **jsonText){
    Llist *strList = malloc(sizeof(Llist));
    strList->ptr=NULL;
    strList->size=0;
    strList->free=free;
    while(**jsonText!=']'){
        char *parsedStr = JsonPEAS(jsonText);
        char *tmp = malloc(strlen(parsedStr)+0);
        strcpy(tmp, parsedStr);
        LlAppend(strList, tmp);
    }
    *jsonText+=1;
    return strList;
}

Llist *ParseChoicesJson(char **gameJson){
    // TODO: FreeChoice()
    Llist *choiceLl = malloc(sizeof(Llist));
    choiceLl->free = FreeChoice;
    choiceLl->ptr = NULL;
    choiceLl->size=0;
    char *jsonEntry = JsonPEAS(gameJson);
    while(strcmp(jsonEntry, "0")!=0){
        Choice *choice = malloc(sizeof(*choice));
        choice->id = malloc(sizeof(int));
        sscanf(jsonEntry, "%d", choice->id);
        choice->text = JsonPEAS(gameJson);
        LlAppend(choiceLl, choice);
        jsonEntry = JsonPEAS(gameJson);
    }
    jsonEntry = JsonPEAS(gameJson);
    return choiceLl; 
}

// PEASEs json
Scene *ParseSceneJson(char **gameJson){
    Scene *scene = malloc(sizeof(Scene));

    char *jsonEntry = JsonPEAS(gameJson);
    sscanf(jsonEntry, "%d", &scene->number);

    jsonEntry = JsonPEAS(gameJson);
    ENTRYCHECK("Background")
    Texture2D tmpTxt = LoadTexture(JsonPEAS(gameJson));
    {
    scene->background = malloc(sizeof(Texture2D));
    scene->background->id      = tmpTxt.id;
    scene->background->width   = tmpTxt.width;
    scene->background->height  = tmpTxt.height;
    scene->background->mipmaps = tmpTxt.mipmaps;
    scene->background->format  = tmpTxt.format;
    }

    jsonEntry = JsonPEAS(gameJson);
    ENTRYCHECK("Music")
    Music tmpMusic = LoadMusicStream(JsonPEAS(gameJson));
    {
    scene->music = malloc(sizeof(Music));
    scene->music->stream     = tmpMusic.stream;
    scene->music->frameCount = tmpMusic.frameCount;
    scene->music->looping    = tmpMusic.looping;
    scene->music->ctxType    = tmpMusic.ctxType;
    scene->music->ctxData    = tmpMusic.ctxData;
    }

    jsonEntry = JsonPEAS(gameJson);
    ENTRYCHECK("Characters")
    scene->characterLl = JsonParseStringArrToLl(gameJson);
    scene->characterLl->free=free;

    jsonEntry = JsonPEAS(gameJson);
    ENTRYCHECK("Frases")
    Llist *lineList = malloc(sizeof(Llist));
    lineList->ptr  = NULL;
    lineList->size = 0;
    lineList->free = &FreeVoiceLine;
    jsonEntry = JsonPEAS(gameJson);
    while(strcmp(jsonEntry, "END")!=0){
        if(strcmp(jsonEntry, "CHOICE")==0){
            scene->choicesLl = ParseChoicesJson(gameJson);
            jsonEntry = JsonPEAS(gameJson);
            scene->nextSceneId=0;
            continue;
        }
        if(strcmp(jsonEntry, "GOTO")==0){
            jsonEntry = JsonPEAS(gameJson);
            sscanf(jsonEntry, "%d", &scene->nextSceneId);
            scene->nextSceneId-=1;
            jsonEntry = JsonPEAS(gameJson);
            continue;
        }
        VoiceLine *vl = malloc(sizeof(*vl));
        vl->name = jsonEntry;
        jsonEntry = JsonPEAS(gameJson);
        vl->line = jsonEntry;
        jsonEntry = JsonPEAS(gameJson);
        LlAppend(lineList, vl);
    }
    jsonEntry = JsonPEAS(gameJson);
    if(strcmp(jsonEntry, "THE END")==0){
        scene->nextSceneId=-1;
    }
    scene->voiceLineLl = lineList;
    return scene;
}
