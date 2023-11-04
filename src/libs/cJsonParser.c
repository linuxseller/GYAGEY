#include "cJsonParser.h"
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

