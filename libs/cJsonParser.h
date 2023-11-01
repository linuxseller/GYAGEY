#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LL_IMPL
#define LL_IMPL
#include "Ll.h"
#endif

#ifndef CLOGGER_IMPL
#define CLOGGER_IMPL
#include "clogger.h"
#endif

typedef struct {
    char* key;
    char *value;
} JsonPairString;

char *JsonText;
#ifdef JSON_IMPL
char *JsonReadWholeFile(FILE *f){
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    JsonText=string;
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
#ifdef JSONLOG
    clogger(CLOGGER_INFO, "json succesfully parsed '%s'", key);
#endif
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
    while(**jsonText!=']'){
        char *parsedStr = JsonPEAS(jsonText);
        char *tmp = malloc(strlen(parsedStr)+0);
        strcpy(tmp, parsedStr);
        LlAppend(strList, tmp);
    }
    *jsonText+=1;
    return strList;
}
#endif
