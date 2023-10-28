#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> 

int digit(int a){
    int res=1;
    for(int i=0; i<a; i++){
        res*=10;
    }
    return res;
}

int IsValidData(char *data){
    int i=0, num = 0;
    for(i=0; isdigit(data[i]); i++){
        num+=digit(i)*data[i]-48;
    }

    return (num)?num:-1;
}

FILE *fptr;
int prevLineNum;

int PrepareFile(const char *fileName){
    fptr = fopen(fileName, "r");
    return (fptr==NULL)?-1:0;
}

int GetNextLineData(char **name, char **text){
    char *myString=malloc(140*sizeof(char));
    if(fgets(myString, 140, fptr)==NULL){
        return -1;
    }
    char *strStartPtr = myString;
    while(*strStartPtr!=':'){
        strStartPtr+=1;
    }
    *strStartPtr=0;
    strStartPtr+=1;
    *text = strStartPtr;
    *name = myString;
    return 0;
}

