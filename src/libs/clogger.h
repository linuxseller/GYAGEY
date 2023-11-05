#pragma once
#include <stdio.h>
#define CLOGGER_INFO 0 //"[INFO ] "
#define CLOGGER_WARN 1 //"[WARN ] "
#define CLOGGER_ERROR 2 //"[ERROR] "
typedef int Clogger_type;

void fclogger(FILE* fp, Clogger_type type, const char *fmt, ...);
void clogger(Clogger_type type, const char *str, ...);

#ifdef CLOGGER_IMPL
void fclogger(FILE* fp, Clogger_type type, const char *fmt, ...){
    switch(type){
        case CLOGGER_INFO:
            fprintf(fp, "[INFO] ");
            break;
        case CLOGGER_WARN:
            fprintf(fp, "[WARN] ");
            break;
        case CLOGGER_ERROR:
            fprintf(fp, "[ERROR] ");
            break;
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    fprintf(fp, "\n");
}
void clogger(Clogger_type type, const char *str, ...){
    FILE *fp = stderr;
    switch(type){
        case CLOGGER_INFO:
            fprintf(fp, "[INFO] ");
            break;
        case CLOGGER_WARN:
            fprintf(fp, "[WARN] ");
            break;
        case CLOGGER_ERROR:
            fprintf(fp, "[ERROR] ");
            break;
    }
    va_list args;
    va_start(args, str);
    vfprintf(fp, str, args);
    va_end(args);
    fprintf(fp, "\n");

}
#endif
