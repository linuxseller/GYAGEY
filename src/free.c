#include "free.h"
// function for passing to LlFree(void *ptr)
void FreeVoiceLine(void *ptr){
    VoiceLine *vl = (VoiceLine*)ptr;
    free(vl->name);
    free(vl->line);
}
// function for passing to LlFree(void *ptr)
void FreeChoice(void *ptr){
    Choice *vl = (Choice*)ptr;
    free(vl->id);
    free(vl->text);
}
// function for passing to LlFree(void *ptr)
void FreeScene(void *ptr){
    Scene *scene = (Scene*)ptr;
    LlFree(scene->characterLl);
    LlFree(scene->voiceLineLl);
    if(scene->nextSceneId==0){
        LlFree(scene->choicesLl);
    }
    UnloadTexture(*scene->background);
    UnloadMusicStream(*scene->music);
}

