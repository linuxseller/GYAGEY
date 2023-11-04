#include "AssList.h"

AssList AssInit(void){
    AssList assListObj = LlInit();
    return assListObj;
}

void AssAppend(AssList *list, char *key, void *value){
    AssPair *tmpAssPair = malloc(sizeof(*tmpAssPair));
    tmpAssPair->key = key;
    tmpAssPair->value = value;
    LlAppend(list, tmpAssPair);
}

void* AssGet(AssList *list, char *key){
    int i=0;
    AssPair *tmp;
    do {
        tmp = LlGetAt(list, i);
        i++;
    } while(strcmp(tmp->key, key)!=0 && i<list->size);
    return tmp->value;
}

