#include "scan.h"
#include "idlist.h"

static char rbuf;
FILE *fp;
int linenum, isnewline;

//fp error handle is not executed
/* File 'fileHandle.c'
Lines executed:100.00% of 20
Branches executed:100.00% of 16
Taken at least once:87.50% of 16
Calls executed:100.00% of 6
Creating 'fileHandle.c.gcov' */

// open source file
int init_scan(char *filename){
    fp = fopen(filename, "r");
    linenum = 1 ;
    isnewline = 0;
    init_idtab();
    if(fp <= 0) return -1;
    else return 0;
}

// close file stream
void end_scan(){
    fclose(fp);
    release_idtab();
}

//read 1 letter and if linebreak, count increment and return '\n'
//When EOF, return EOF;
//other error, return '\0'
char read_char(){
    //if new line, add linenum
    if(isnewline) linenum++;
    isnewline = 0;
    if(!(rbuf = fgetc(fp))){
        if(feof(fp)) return EOF;
        else return '\0';
    }else{
        if(rbuf == '\n' || rbuf == '\r') {
            char _rbuf = rbuf; 
            isnewline = 1;
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

