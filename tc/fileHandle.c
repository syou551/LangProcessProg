#include "scan.h"

static char rbuf;
FILE *fp;
int linenum;

// open source file
int init_scan(char *filename){
    fp = fopen(filename, "r");
    linenum = 1 ;
    if(fp == 0) return -1;
    else return 0;
}

// close file stream
void end_scan(){
    fclose(fp);
}

//read 1 letter and if linebreak, count increment and return '\n'
//When EOF, return EOF;
//other error, return '\0'
char read_char(){
    if(!(rbuf = fgetc(fp))){
        if(feof(fp)) return EOF;
        else return '\0';
    }else{
        if(rbuf == '\n' || rbuf == '\r') {
            char _rbuf = rbuf; 
            linenum++;
            if(!(rbuf = fgetc(fp))){
                if(feof(fp)) return EOF;
                else return '\0';
            }else if(_rbuf == '\r'){
                if(rbuf != '\n') ungetc(rbuf,fp);
            }else if(_rbuf == '\n'){
                if(rbuf != '\r') ungetc(rbuf,fp);
            }
            return '\n';
        }
        return rbuf;
    }
}

int get_linenum(void){
    return linenum;
}

