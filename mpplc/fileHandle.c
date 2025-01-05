#include "scan.h"
#include "idlist.h"
#include "makeCode.h"

static char rbuf;
FILE *fp;
FILE *cslfp;
int linenum, isnewline;


char *get_cslfilename(char *np){
    char *slash = strrchr(np,'/');
    if(slash == NULL) return NULL;
    char *newnp = NULL;
    if((newnp = (char *)malloc(strlen(slash++))) == NULL) return NULL;
    snprintf(newnp,strlen(slash)+1,"%s",slash);
    char *dot = strrchr(newnp, '.');
    if(dot == NULL) {
        char *newnp_with_extension = NULL;
        if((newnp_with_extension = (char *)malloc(strlen(newnp) + 1 + 4)) == NULL) return NULL;
        snprintf(newnp_with_extension,sizeof(newnp_with_extension),"%s.csl",newnp);
        free(newnp);
        return newnp_with_extension;
    }
    
    snprintf(dot,5,".csl");
    return newnp;
}

// open source file
int init_scan(char *filename){
    fp = fopen(filename, "r");
    linenum = 1;
    isnewline = 0;
    init_idtab();
    if(fp <= 0) return -1;

    char *fname = NULL, *path = NULL;
    fname = get_cslfilename(filename);
    if((path = (char *)malloc(strlen(fname) + 2)) == NULL) return error("ERROR: can't get memory spape");
    snprintf(path,3,"./");
    strcat(path, fname);
    free(fname);

    printf("%s",path);
    cslfp = fopen(path, "w");
    free(path);
    return 0;
}

// close file stream
void end_scan(){
    fclose(fp);
    fclose(cslfp);
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

