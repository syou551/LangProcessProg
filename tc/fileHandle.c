#include "scan.h"

static char rbuf;
FILE *fp;
int linenum;

// open source file
int init_scan(char *filename){
    fp = fopen(filename, "r");
    linenum = 0;
    if(fp == 0) return -1;
    else return 0;
}

// close file stream
void end_scan(){
    fclose(fp);
}

//read 1 letter
//When EOF, return EOF;
//other error, return '\0'
char read_char(){
    if(!(rbuf = fgetc(fp))){
        if(feof(fp)) return EOF;
        else return '\0';
    }else return rbuf;
}

