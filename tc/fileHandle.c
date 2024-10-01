#include "scan.h"

static char rbuf[1];

// open source file
int init_scan(char *filename){
    fp = fopen(filename, "r");

    if(fp == 0) return -1;
    else return 0;
}

// close file stream
void end_scan(){
    fclose(fp);
}

//read 1 letter
//When end of line, automatically go to next line
char read_char(){
    if(!fread(rbuf,sizeof(char),1,fp))return '\0';
    else return rbuf[0];
}

